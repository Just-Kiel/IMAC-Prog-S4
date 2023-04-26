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
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "obstacle/obstacle.hpp"
#include "p6/p6.h"
#include "perfs/perfs.hpp"

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

    // TODO fix Cell info
    float cellSize = 1.f;

    // Actual app
    auto ctx = p6::Context{{.title = "Projecto con Olivia"}};
    ctx.maximize_window();

    // Shaders initialization
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        //"shaders/normals.fs.glsl"
        "shaders/pointLight.fs.glsl"
    );

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // Models initialization
    ModelsLOD modelBoidsLOD({"assets/models/untitled.obj", "assets/models/test.obj", "assets/models/cone.obj"});

    // Models initialization
    ModelsLOD modelObstacleLOD({"assets/models/sphere.obj", "assets/models/sphere.obj", "assets/models/sphere.obj"});

    // Model initialization
    Model cellGlobal("assets/models/cell.obj");

    ModelParams cellParams{
        .center    = glm::vec3(0, -1, 0),
        .scale     = 1.f,
        .direction = glm::vec3(-1, 0, 0),
    };

    // Camera
    FreeflyCamera camera;

    // Boids instance
    std::vector<Boid> allBoids = createBoids();
    Perfs             boidPerformances{};

    // Obstacles
    std::vector<Obstacle> allObstacles;

    // ImGui informations
    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids parameters");
        ImGui::SliderFloat("Boids speed", &speed, 0.f, 2.f);

        boidPerformances.ImGui();

        // Boids
        ImguiBoids(allBoids);

        // Forces
        globalForces.ImGui();

        // Obstacles
        if (ImGui::Button("Clear Obstacles"))
        {
            allObstacles.clear();
        }
        ImGui::End();

        // Show another simple window, this time using an explicit Begin/End pair
        ImGui::Begin("Cell zone");
        ImGui::SliderFloat("Cell radius", &cellParams.scale, 1.f, 5.f);
        ImGui::End();
    };

    // Declare your infinite update loop
    ctx.update = [&]() {
        // For both shadow mapping and rendering
        const glm::mat4 ViewMatrix = camera.getViewMatrix();
        const auto      cameraPos  = glm::vec3(ViewMatrix[3]);

        // Compute boids parameters for drawing model
        const std::vector<ModelParams> paramsAllBoids = computeBoidsParams(allBoids, cameraPos);

        // Compute obstacles parameters for drawing model
        std::vector<ModelParams> paramsAllObstacles = computeObstaclesParams(allObstacles, cameraPos);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        const glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        // lighting (material)
        shader.set("uKd", glm::vec3{0.9f, 0.8f, 0.9f});
        shader.set("uKs", glm::vec3{0.2f, 0.3f, 0.2f});
        shader.set("uShininess", 100.f);

        // lighting (light)
        shader.set("uLightIntensity", glm::vec3{2.f, 2.f, 2.f});

        // Positions Lights
        std::array pointLightPositions = {
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-0.7f, -0.2f, -2.0f)};

        // point light 1
        shader.set("u_lightsPos[0]", pointLightPositions[0]);
        // point light 2
        shader.set("u_lightsPos[1]", pointLightPositions[1]);

        {
            // Boids need to be in the box so the area of moving must be smaller than the actual box size
            const float backupCellSize = cellParams.scale;
            cellParams.scale *= 1.5f;
            cellGlobal.drawModel(shader, ProjMatrix, ViewMatrix, cellParams);
            cellParams.scale = backupCellSize;
        }

        boidPerformances.startTimer();

        for (auto& boid : allBoids)
        {
            boid.avoidWalls(cellParams.scale);
            boid.avoidObstacles(allObstacles);
            boid.updateCenter(speed, globalForces, allBoids);
        }

        boidPerformances.TimerComputeBoids();

        boidPerformances.startTimer();

        for (auto const& boid : paramsAllBoids)
        {
            modelBoidsLOD.drawModel(shader, ProjMatrix, ViewMatrix, boid);
        }

        boidPerformances.TimerDrawBoids();

        for (auto const& obstacle : paramsAllObstacles)
        {
            modelObstacleLOD.drawModel(shader, ProjMatrix, ViewMatrix, obstacle);
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

    ctx.key_pressed = [&](const p6::Key& key) {
        cameraKeyControls(key, camera);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}