#include <vcruntime.h>
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
            glm::vec3(distribution(gen) * ctx.aspect_ratio(), distribution(gen), 0),
            p6::Radius(0.1f),
            static_cast<p6::Rotation>(2.0_radians * p6::PI * distribution(gen))
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
    std::default_random_engine            gen;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    // Actual app
    auto ctx = p6::Context{{.title = "Projecto con Olivia"}};
    ctx.maximize_window();

    // Boids instance
    std::vector<Boid> allBoids = createBoids(ctx);

    // ImGui informations
    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("Boids parameters");
        ImGui::SliderFloat("Boids speed", &speed, 0.f, 2.f);

        {
            int nbBoids = allBoids.size();
            if (ImGui::SliderInt("Boids number", &nbBoids, 0, 500))
            {
                if (nbBoids > allBoids.size())
                {
                    allBoids.emplace_back(glm::vec3(distribution(gen) * ctx.aspect_ratio(), distribution(gen), 0), p6::Radius(0.1f), static_cast<p6::Rotation>(2.0_radians * p6::PI * distribution(gen)));
                }
                else
                {
                    allBoids.pop_back();
                }
            }
        }
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        for (auto& boid : allBoids)
        {
            boid.updateCenter(speed, allBoids);
            boid.avoidWalls(ctx);
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