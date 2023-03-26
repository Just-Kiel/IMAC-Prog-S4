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
#include "imgui.h"
#include "obstacle/obstacle.hpp"
#include "p6/p6.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

std::vector<Boid> createBoids(p6::Context& ctx)
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

    // Actual app
    auto ctx = p6::Context{{.title = "Projecto con Olivia"}};
    ctx.maximize_window();

    // Camera
    FreeflyCamera camera;

    // Boids instance
    std::vector<Boid>             allBoids = createBoids(ctx);
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
            int   nbBoids = allBoids.size();
            float radius  = nbBoids != 0 ? allBoids[0].radius() : 0.1f;

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
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();
        // ctx.background(p6::NamedColor::Blue);

        auto start = std::chrono::system_clock::now();

        for (auto& boid : allBoids)
        {
            boid.updateCenter(speed, allBoids);
            boid.avoidWalls(ctx);
            // boid.updateDir(speed, static_cast<p6::Angle>(0.05_radians * p6::PI * distribution(gen)));
        }

        elapsed_update_seconds = std::chrono::system_clock::now() - start;

        start = std::chrono::system_clock::now();
        for (auto& boid : allBoids)
        {
            boid.draw(ctx, shader, ProjMatrix, ViewMatrix);
        }
        elapsed_draw_seconds = std::chrono::system_clock::now() - start;

        for (auto& obstacle : allObstacles)
        {
            obstacle.draw(ctx);
        }
    };
    // Mouse
    ctx.mouse_pressed = [&](p6::MouseButton button) {
        if (button.button == p6::Button::Left)
        {
            std::cout << "x = " << ctx.mouse()[0] << " y = " << ctx.mouse()[1] << std::endl;
            allObstacles.emplace_back(button.position.x, button.position.y, p6::Radius{0.03f});
        }
    };

    // Camera controls
    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 5.f);
        camera.rotateUp(drag.delta.y * 5.f);
    };

    ctx.key_pressed = [&](p6::Key key) {
        std::cout << "Key pressed: " << key.logical << std::endl;

        if (key.logical == "q")
        {
            camera.moveLeft(5.f);
        }
        else if (key.logical == "d")
        {
            camera.moveLeft(-5.f);
        }
        else if (key.logical == "z")
        {
            camera.moveFront(1.f);
        }
        else if (key.logical == "s")
        {
            camera.moveFront(-1.f);
        }
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
}