#pragma once
#include <array>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 centeredCoord{};

    p6::Radius m_radius;

    p6::Rotation m_rotation;

public:
    // Boid();
    Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation);

    ~Boid() = default;

    void draw(p6::Context& ctx);

    void updateCenter(float speed);

    void updateDir(float speed, p6::Angle angle);
};