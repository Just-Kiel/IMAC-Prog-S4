#include <cstdlib>
#include <random>
#include <vector>
#include "glm/fwd.hpp"
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
    size_t nbSquare = 100;

    std::vector<glm::vec2>                coordSquare(nbSquare);
    std::vector<glm::vec2>                directionSquare(nbSquare);
    std::default_random_engine            gen;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);
    for (size_t i = 0; i < nbSquare; ++i)
    {
        coordSquare[i]     = glm::vec2(distribution(gen) * 2.0f, distribution(gen));
        directionSquare[i] = glm::vec2(distribution(gen) / 10, distribution(gen) / 10);
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Prog-S4"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        for (size_t i = 0; i < nbSquare; ++i)
        {
            coordSquare[i] += directionSquare[i] * 0.1f;
            ctx.square(
                p6::Center{coordSquare[i]},
                p6::Radius{0.1f}
            );
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}