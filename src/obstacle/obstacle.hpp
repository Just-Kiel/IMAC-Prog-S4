#pragma once
#include "p6/p6.h"

class Obstacle {
private:
    float      m_x;
    float      m_y;
    p6::Radius m_radius;

public:
    Obstacle(float x, float y, p6::Radius radius);

    ~Obstacle() = default;

    void draw(p6::Context& ctx);

    p6::Radius getRadius() const;

    glm::vec3 getPosition() const;
};