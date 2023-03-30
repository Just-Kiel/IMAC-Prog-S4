#include "obstacle.hpp"
#include <vector>
#include "detections/detections.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "obstacle/obstacle.hpp"

Obstacle::Obstacle(float x, float y, p6::Radius radius)
    : m_x(x), m_y(y), m_radius(radius){};

void Obstacle::draw(p6::Context& ctx)
{
    ctx.circle(
        p6::Center{m_x, m_y},
        m_radius
    );
}

p6::Radius Obstacle::getRadius() const
{
    return m_radius;
}

glm::vec3 Obstacle::getPosition() const
{
    return {m_x, m_y, 0};
}