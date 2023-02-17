#pragma once
#include <vcruntime.h>
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

    // std::vector<glm::vec3> coordPointsTriangle{3};
    // std::array<glm::vec3, 3> coordPointsTriangle;

public:
    // Boid();
    Boid(glm::vec3 center, p6::Radius radius);

    ~Boid() = default;

    void draw(p6::Context& ctx);

    void updateCenter(glm::vec3 move, float speed);
};