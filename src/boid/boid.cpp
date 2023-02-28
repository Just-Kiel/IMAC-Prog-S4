#include "boid.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"

// Boid::Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation)
//     : centeredCoord(center), m_radius(radius), m_rotation(rotation), m_direction(p6::rotated_by(rotation, glm::vec2(1., 0.)), 0.){

//     };

Boid::Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation)
    : centeredCoord(center), m_radius(radius), m_rotation(rotation)
{
    m_direction = glm::vec3(p6::rotated_by(rotation, glm::vec2(1., 0.)), 0.);
};

void Boid::draw(p6::Context& ctx)
{
    ctx.equilateral_triangle(
        p6::Center{centeredCoord},
        m_radius,
        p6::Angle(glm::vec2(m_direction))
    );
}

void Boid::updateCenter(float speed, const std::vector<Boid>& neighbors)
{
    // Add separation process
    m_direction += this->separation(neighbors) * speed;

    // glm::vec3 move = glm::vec3(p6::rotated_by(m_rotation, glm::vec2(1., 0.)), 0.);
    centeredCoord += m_direction * speed / 100.f;
}

void Boid::updateDir(float speed, p6::Angle angle)
{
    m_rotation += angle * speed;
}

glm::vec3 Boid::separation(const std::vector<Boid>& neighbors)
{
    // TODO(Aurore): max distance à déterminer
    float maxDistance = 0.03f;

    // Count of boids to close from current boid
    size_t    count = 0;
    glm::vec3 total = glm::vec3(0, 0, 0);

    // Check for every boids if its to close
    for (auto& boid : neighbors)
    {
        if (&boid != this)
        {
            // Calculate distance
            float distance = this->distance(boid);

            if (distance < maxDistance)
            {
                // Difference in term of position not length
                glm::vec3 difference = this->centeredCoord - boid.centeredCoord;

                glm::vec3 move = glm::normalize(difference) / static_cast<float>(difference.length());

                total += move;
                count++;
            }
        }
    }

    if (count != 0)
    {
        total /= count;
    }
    return total;
}

float Boid::distance(const Boid& anotherBoid)
{
    return glm::distance2(this->centeredCoord, anotherBoid.centeredCoord);
}