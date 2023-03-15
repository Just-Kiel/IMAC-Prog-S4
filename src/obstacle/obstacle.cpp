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
    ctx.square(
        p6::Center{m_x, m_y},
        m_radius,
        p6::Rotation{0_turn}
    );
}