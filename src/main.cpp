#include <cstdlib>
#include <random>
#include <vector>
#include "boid/boid.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "imgui.h"
#include "p6/p6.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

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

    // Squares infos
    int   nbSquare = 100;
    float speed    = 0.001f;

    std::vector<glm::vec2>                coordSquare(nbSquare);
    std::vector<glm::vec2>                directionSquare(nbSquare);
    std::default_random_engine            gen;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);
    for (size_t i = 0; i < nbSquare; ++i)
    {
        coordSquare[i]     = glm::vec2(distribution(gen) * 2.0f, distribution(gen));
        directionSquare[i] = glm::vec2((distribution(gen) > 0) ? 1 : -1, (distribution(gen) > 0) ? 1 : -1);
    }

    // Boids test
    // TODO(Aurore): maybe not init there
    std::vector<Boid> allBoids;

    for (size_t i = 0; i < nbSquare; ++i)
    {
        allBoids.emplace_back(glm::vec3(coordSquare[i], 0), p6::Radius(0.1f), static_cast<p6::Rotation>(2.0_radians * p6::PI * distribution(gen)));
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Projecto con Olivia"}};
    ctx.maximize_window();

    // ImGui informations
    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids parameters");
        ImGui::SliderFloat("Boids speed", &speed, 0.f, 2.f);
        ImGui::SliderInt("Boids number", &nbSquare, 0, 500);
        ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        // ImGui::ShowDemoWindow();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        if (nbSquare != allBoids.size())
        {
            if (nbSquare > allBoids.size())
            {
                allBoids.emplace_back(glm::vec3(distribution(gen) * 2.0f, distribution(gen), 0), p6::Radius(0.1f), static_cast<p6::Rotation>(2.0_radians * p6::PI * distribution(gen)));
            }
            else
            {
                allBoids.pop_back();
            }
        }

        for (auto& boid : allBoids)
        {
            boid.updateCenter(speed);
            // boid.updateDir(speed, static_cast<p6::Angle>(0.05_radians * p6::PI * distribution(gen)));
        }

        for (auto& boid : allBoids)
        {
            boid.draw(ctx);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}