#pragma once
#include <array>
#include <vector>
#include "forces/forces.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 centeredCoord{};

    float m_radius;

    glm::vec3 m_direction;

    Forces m_forces{};

public:
    // Boid();
    // Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation);

    ~Boid() = default;

    inline float radius() const { return m_radius; }

    void setForces(Forces forces);

    void draw(p6::Context& ctx);

    void updateCenter(float speed, const std::vector<Boid>& neighbors);

    glm::vec3 separation(const std::vector<Boid>& neighbors);
    glm::vec3 alignment(const std::vector<Boid>& neighbors);
    glm::vec3 cohesion(const std::vector<Boid>& neighbors);

    float distance(const Boid& anotherBoid) const;

    void avoidObstacles();

    void avoidWalls(p6::Context& ctx);

    void rotateLeft();

    void rotateRight();
};