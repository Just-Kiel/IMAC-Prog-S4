#include "boid.hpp"

Boid::Boid(glm::vec3 center, p6::Radius radius)
    : centeredCoord(center), m_radius(radius){};

void Boid::draw(p6::Context& ctx)
{
    ctx.equilateral_triangle(
        p6::Center{centeredCoord},
        m_radius
    );
}

void Boid::updateCenter(glm::vec3 move, float speed)
{
    centeredCoord += move * speed / 100.f;
}