#include <vcruntime.h>
#include <chrono>
#include <cstdlib>
#include <random>
#include <vector>
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

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

std::vector<Boid> createBoids(ModelsLOD& boidModel)
{
    size_t nbBoids = 2;

    std::default_random_engine            gen;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    // Boids init
    std::vector<Boid> allBoids;

    for (size_t i = 0; i < nbBoids; ++i)
    {
        allBoids.emplace_back(
            glm::vec3(distribution(gen), distribution(gen), distribution(gen)),
            0.1f,
            boidModel
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
    float                                 speed = 0.001f;
    Forces                                globalForces{1.f, 0.25f, 1.f};
    std::default_random_engine            gen;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    // Cell info
    float cellSize = 1.f;

    // Actual app
    auto ctx = p6::Context{{.title = "Projecto con Olivia"}};
    ctx.maximize_window();

    // Shaders initialization
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Models initialization
    ModelsLOD modelsLOD({"assets/models/untitled.obj", "assets/models/test.obj", "assets/models/cone.obj"});
    modelsLOD.initModels();

    // Models initialization
    ModelsLOD modelObstacleLOD({"assets/models/sphere.obj", "assets/models/sphere.obj", "assets/models/sphere.obj"});
    modelObstacleLOD.initModels();

    // Camera
    FreeflyCamera camera;

    // Boids instance
    std::vector<Boid>             allBoids = createBoids(modelsLOD);
    std::chrono::duration<double> elapsed_update_seconds;
    std::chrono::duration<double> elapsed_draw_seconds;

    // Obstacles
    std::vector<Obstacle> allObstacles;

    // ImGui informations
    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids parameters");
        ImGui::SliderFloat("Boids speed", &speed, 0.f, 2.f);

        ImGui::LabelText("", "Elapsed update time: %fms", elapsed_update_seconds.count() * 1000.0);
        ImGui::LabelText("", "Elapsed draw time: %fms", elapsed_draw_seconds.count() * 1000.0);

        {
            glm::int32 nbBoids = allBoids.size();
            float      radius  = nbBoids != 0 ? allBoids[0].radius() : 0.1f;

            if (ImGui::SliderInt("Boids number", &nbBoids, 0, 5000))
            {
                while (nbBoids > allBoids.size())
                {
                    allBoids.emplace_back(
                        glm::vec3(distribution(gen), distribution(gen), distribution(gen)),
                        radius,
                        modelsLOD
                    );
                }

                while (nbBoids < allBoids.size())
                {
                    allBoids.pop_back();
                }
            }

            if (ImGui::SliderFloat("Boids radius", &radius, 0.f, 1.f))
            {
                allBoids.clear();

                for (size_t i = 0; i < nbBoids; ++i)
                {
                    allBoids.emplace_back(
                        glm::vec3(distribution(gen), distribution(gen), distribution(gen)),
                        radius,
                        modelsLOD
                    );
                }
            }
        }

        // Forces
        {
            if (globalForces.ImGui())
            {
                for (auto& boid : allBoids)
                {
                    boid.setForces(globalForces);
                }
            }
        }

        // Obstacles
        if (ImGui::Button("Clear Obstacles"))
        {
            allObstacles.clear();
        }
        ImGui::End();

        // Show another simple window, this time using an explicit Begin/End pair
        ImGui::Begin("Cell zone");
        ImGui::SliderFloat("Cell radius", &cellSize, 0.f, 25.f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();

        auto start = std::chrono::system_clock::now();

        for (auto& boid : allBoids)
        {
            boid.updateCenter(speed, allBoids);
            boid.avoidWalls(cellSize);
            boid.avoidObstacles(allObstacles);
        }

        elapsed_update_seconds = std::chrono::system_clock::now() - start;

        start = std::chrono::system_clock::now();
        for (auto& boid : allBoids)
        {
            boid.draw(shader, ProjMatrix, ViewMatrix);
        }
        elapsed_draw_seconds = std::chrono::system_clock::now() - start;

        for (auto& obstacle : allObstacles)
        {
            obstacle.draw(shader, ProjMatrix, ViewMatrix);
        }
    };
    // Obstacles controls
    ctx.mouse_pressed = [&](p6::MouseButton button) {
        bool OnOtherObstacle = false;
        if (button.button == p6::Button::Right)
        {
            for (auto& obstacle : allObstacles)
            {
                std::cout << "x = " << obstacle.getPosition()[0] << std::endl;
                std::cout << "y = " << obstacle.getPosition()[1] << std::endl;
                std::cout << "z = " << obstacle.getPosition()[2] << std::endl;
                float dist = glm::distance(glm::vec2(obstacle.getPosition()[0], obstacle.getPosition()[1]), glm::vec2(button.position.x, button.position.y));
                if (dist <= 2 * obstacle.getRadius().value)
                {
                    OnOtherObstacle = true;
                }
            }
            if (!OnOtherObstacle)
            {
                // TODO(Olivia) change in function cell size / view matrix
                allObstacles.emplace_back(glm::vec3{-button.position.x, button.position.y, 0}, 0.1f, modelObstacleLOD);
            }
            std::cout << OnOtherObstacle << std::endl;
        }
    };

    // Camera controls
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 5.f);
        camera.rotateUp(drag.delta.y * 5.f);
    };

    ctx.key_pressed = [&](p6::Key key) {
        cameraKeyControls(key, camera);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}