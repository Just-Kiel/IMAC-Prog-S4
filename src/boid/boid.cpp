#include "boid.hpp"
#include "glm/fwd.hpp"

Boid::Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation)
    : centeredCoord(center), m_radius(radius), m_rotation(rotation){};

void Boid::draw(p6::Context& ctx)
{
    ctx.equilateral_triangle(
        p6::Center{centeredCoord},
        m_radius,
        m_rotation
    );
}

void Boid::updateCenter(float speed)
{
    glm::vec3 move = glm::vec3(p6::rotated_by(m_rotation, glm::vec2(1., 0.)), 0.);
    centeredCoord += move * speed / 100.f;
}

void Boid::updateDir(float speed, p6::Angle angle)
{
    m_rotation += angle * speed;
}