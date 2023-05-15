#include <chrono>
#include <cstdlib>
#include <random>
#include <vector>
#include "LOD/LOD.hpp"
#include "boid/boid.hpp"
#include "forces/forces.hpp"
#include "freeflyCamera/freeflyCamera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "imgui.h"
#include "imgui/gui.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "obstacle/obstacle.hpp"
#include "opengl/textureOpenGl.hpp"
#include "p6/p6.h"
#include "perfs/perfs.hpp"
#include "shadowMapping/shadowMapping.hpp"
#include "skybox/skybox.hpp"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

std::vector<Boid> createBoids()
{
    size_t nbBoids = 1;

    // Boids init
    std::vector<Boid> allBoids;

    for (size_t i = 0; i < nbBoids; ++i)
    {
        allBoids.emplace_back(
            glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
            0.01f
        );
    }

    return allBoids;
}

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Speed info
    float  speed = 0.001f;
    Forces globalForces{
        .m_separationForce = 1.f,
        .m_cohesionForce   = 0.25f,
        .m_alignmentForce  = 1.f,
    };

    // Actual app
    auto ctx = p6::Context{{.title = "Paperplane simulation"}};
    ctx.maximize_window();

    // Shaders initialization
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        //"shaders/normals.fs.glsl"
        "shaders/pointLight.fs.glsl"
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Models initialization
    ModelsLOD modelBoidsLOD({"assets/models/paperplane_low.obj", "assets/models/paperplane_medium.obj", "assets/models/paperplane_high.obj"});

    // Models initialization
    ModelsLOD modelObstacleLOD({"assets/models/sphere.obj", "assets/models/sphere.obj", "assets/models/sphere.obj"});

    // Model initialization
    Model cellGlobal("assets/models/cell.obj");

    ModelParams cellParams{
        .center    = glm::vec3(0, 0, 0),
        .scale     = 1.f,
        .direction = glm::vec3(1, 0, 0),
    };

    float cellGap = 1.5f;

    img::Image cellTex = p6::load_image_buffer("assets/textures/texture_cell.png");
    Texture    cellTexture{};
    cellTexture.image2D(static_cast<int>(cellTex.width()), static_cast<int>(cellTex.height()), cellTex.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    // CubeMap
    Texture                 cubemapTexture{};
    std::vector<img::Image> facesImg;
    // cubemapTexture.cubemap(faces, GL_RGB, GL_UNSIGNED_BYTE);
    p6::Shader skyboxShader = p6::load_shader("shaders/skybox.vs.glsl", "shaders/skybox.fs.glsl");
    VAO        skyboxVAO{};
    VBO        skyboxVBO{};
    createSkybox(cubemapTexture, facesImg, skyboxVAO, skyboxVBO);
    skyboxShader.use();
    skyboxShader.set("skybox", 0);

    // Camera
    FreeflyCamera camera;

    // Shadow mapping
    ShadowMapping shadowMapping({"shaders/framebuffer.vs.glsl", "shaders/framebuffer.fs.glsl"}, ctx.main_canvas_width(), ctx.main_canvas_height());

    // Boids instance
    std::vector<Boid> allBoids = createBoids();
    Perfs             boidPerformances{};
    static bool       showBoidsWindow = false;

    // Obstacles
    std::vector<Obstacle> allObstacles;

    // ImGui informations
    ctx.imgui = [&]() {
        // Show another simple window, this time using an explicit Begin/End pair
        ImGui::Begin("Cell zone");
        ImGui::SliderFloat("Cell radius", &cellParams.scale, 1.f, 5.f);
        ImGui::End();

        if (showBoidsWindow)
        {
            ImGui::Begin("Boids parameters", &showBoidsWindow);
            ImGui::SliderFloat("Boids speed", &speed, 0.f, 2.f);

            boidPerformances.ImGui();

            // Boids
            ImguiBoids(allBoids);

            // Forces
            globalForces.ImGui();

            ImGui::End();
        }

        // Menu bar
        if (ImGui::BeginMainMenuBar())
        {
            // open a window with the boids parameters
            if (ImGui::BeginMenu("Boids parameters"))
            {
                showBoidsWindow = true;
                ImGui::EndMenu();
            }

            // Obstacles
            obstacleImgui(allObstacles);

            // Shadows
            if (ImGui::BeginMenu("Shadows"))
            {
                // Checkboxes for shadows
                ImGui::Checkbox("Display shadows", &shadowMapping.m_displayShadow);

                ImGui::EndMenu();
            }

            // TODO having 2 hdri and switch between them

            // Camera
            cameraImgui(camera);

            // Controls
            ImguiControls();

            // Credits
            ImguiCredits();

            if (ImGui::MenuItem("Quit"))
            {
                ctx.stop();
            }

            ImGui::EndMainMenuBar();
        }
    };

    // Declare your infinite update loop
    ctx.update = [&]() {
        // For both shadow mapping and rendering
        const glm::mat4 ViewMatrix = camera.getViewMatrix();
        const auto      cameraPos  = glm::vec3(ViewMatrix[3]);
        const glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        // Compute boids parameters for drawing model
        const std::vector<ModelParams> paramsAllBoids = computeBoidsParams(allBoids, cameraPos);

        // Compute obstacles parameters for drawing model
        std::vector<ModelParams> paramsAllObstacles = computeObstaclesParams(allObstacles, cameraPos);

        // Positions Lights
        std::array pointLightPositions = {
            glm::vec3(0.5f, 0.5f, 0.0f), // light that displays the shadow map
            glm::vec3(-0.7f, -0.2f, -2.0f),
        };

        const glm::vec3 lightDir = glm::vec3(0.f, -1.f, 0.2f);

        // Shadow Mapping
        glm::mat4 lightView = glm::lookAt(pointLightPositions[0], pointLightPositions[0] + lightDir, glm::vec3(0, 1, 0));

        const glm::mat4 lightProjection  = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4       lightSpaceMatrix = lightProjection * lightView;
        shadowMapping.render(modelBoidsLOD, paramsAllBoids, ProjMatrix, lightSpaceMatrix);

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.set("uUseShadow", shadowMapping.m_displayShadow);
        shader.set("uUseTexture", false);
        shader.set("uLightSpaceMatrix", lightSpaceMatrix);

        // lighting (material)
        shader.set("uKd", glm::vec3{0.9f, 0.8f, 0.9f});
        shader.set("uKs", glm::vec3{0.2f, 0.3f, 0.2f});
        shader.set("uShininess", 100.f);

        // lighting (light)
        shader.set("uLightIntensity", glm::vec3{1.f, 1.f, 1.f});

        // point light 1
        shader.set("u_lightsPos[0]", pointLightPositions[0]);
        // point light 2
        shader.set("u_lightsPos[1]", pointLightPositions[1]);

        boidPerformances.startTimer();

        for (auto& boid : allBoids)
        {
            Forces tempForces = globalForces;
            boid.avoidObstacles(allObstacles);
            if (boid.avoidWalls(cellParams.scale))
            {
                globalForces = Forces{
                    .m_separationForce = 0.f,
                    .m_cohesionForce   = 0.f,
                    .m_alignmentForce  = 0.f,
                };
            }
            boid.updateCenter(speed, globalForces, allBoids);
            globalForces = tempForces;
        }

        boidPerformances.TimerComputeBoids();

        boidPerformances.startTimer();

        for (auto const& boid : paramsAllBoids)
        {
            shadowMapping.activateTexture(shader);
            modelBoidsLOD.drawModel(shader, ProjMatrix, ViewMatrix, boid);
        }

        boidPerformances.TimerDrawBoids();

        for (auto const& obstacle : paramsAllObstacles)
        {
            shadowMapping.activateTexture(shader);
            modelObstacleLOD.drawModel(shader, ProjMatrix, ViewMatrix, obstacle);
        }

        {
            shader.set("uUseTexture", true);
            shader.set("uTexture", 1);
            cellTexture.activate(1);

            // Boids need to be in the box so the area of moving must be smaller than the actual box size
            const float backupCellSize = cellParams.scale;
            cellParams.scale += cellGap;
            shadowMapping.activateTexture(shader);
            cellGlobal.drawModel(shader, ProjMatrix, ViewMatrix, cellParams);
            cellParams.scale = backupCellSize;
        }

        // Skybox
        {
            glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            auto view = glm::mat4(glm::mat3(camera.getViewMatrix())); // remove translation from the view matrix
            skyboxShader.set("view", view);
            skyboxShader.set("projection", ProjMatrix);
            // skybox cube
            glBindVertexArray(*skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, *cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }

        // Camera controls
        {
            cameraKeyControls(ctx, camera, ctx.delta_time(), cellParams.scale + 0.9f * cellGap);
        }
    };

    // Obstacles controls
    ctx.mouse_pressed = [&](p6::MouseButton button) {
        addObstacle(button, allObstacles);
    };

    // Camera controls
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 50.f);
        camera.rotateUp(drag.delta.y * 50.f);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}