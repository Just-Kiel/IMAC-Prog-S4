#include "boid.hpp"
#include <vector>
#include "detections/detections.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"

// Boid::Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation)
//     : centeredCoord(center), m_radius(radius), m_rotation(rotation), m_direction(p6::rotated_by(rotation, glm::vec2(1., 0.)), 0.){

//     };

// Boid::Boid(glm::vec3 center, p6::Radius radius, p6::Rotation rotation)
//     : centeredCoord(center), m_radius(radius), m_rotation(rotation)
// {
//     m_direction = glm::vec3(p6::rotated_by(rotation, glm::vec2(1., 0.)), 0.);
// };
{
    m_direction = glm::vec3(p6::rotated_by(rotation, glm::vec2(1., 0.)), 0.);
};

void Boid::setForces(Forces forces)
{
    m_forces = forces;
}

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
    // Add cohesion process
    m_direction += this->cohesion(neighbors) * m_forces.m_cohesionForce * speed;

    // Add separation process
    m_direction += this->separation(neighbors) * speed * m_forces.m_separationForce;

    // Add alignment process
    m_direction += this->alignment(neighbors) * speed * m_forces.m_alignmentForce;

    // Limit the speed
    m_direction = glm::normalize(m_direction);

    // glm::vec3 move = glm::vec3(p6::rotated_by(m_rotation, glm::vec2(1., 0.)), 0.);
    centeredCoord += m_direction * speed / 100.f;
}

glm::vec3 Boid::separation(const std::vector<Boid>& neighbors)
{
    // TODO(Aurore): max distance à déterminer
    float maxDistance = m_radius / 2.f;

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

glm::vec3 Boid::alignment(const std::vector<Boid>& neighbors)
{
    float distanceOfVision = m_radius * 2.f;

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

            if (distance < distanceOfVision)
            {
                total += boid.m_direction;
                count++;
            }
        }
    }

    if (count != 0)
    {
        total /= count;
        total = glm::normalize(total);
    }

    return total;
}

float Boid::distance(const Boid& anotherBoid) const
{
    return glm::distance2(this->centeredCoord, anotherBoid.centeredCoord);
}

glm::vec3 Boid::cohesion(const std::vector<Boid>& neighbors)
{
    float distanceAttraction = m_radius * 5.f;

    // Count of boids to close from current boid
    size_t count = 0;

    glm::vec3 averagePos = glm::vec3(0, 0, 0);
    for (auto& boid : neighbors)
    {
        if (&boid != this)
        {
            // Calculate distance
            float distance = this->distance(boid);

            if (distance < distanceAttraction)
            {
                averagePos += boid.centeredCoord;
                count++;
            }
        }
    }

    if (count != 0)
    {
        averagePos /= count;
        averagePos = glm::normalize(averagePos);
    }

    glm::vec3 distancePos = averagePos - this->centeredCoord;

    return distancePos;
}

// TODO(Aurore): voir si le code est le meme que avoid walls
void Boid::avoidObstacles()
{
    // Avoid going outside of window
    // Look forward head at a set distance
    // return other direction if there is something
}

void Boid::avoidWalls(const float& radius)
{
    // Avoid going outside of window
    // Look forward head at a set distance
    // return other direction if there is something

    // Min - max x = -ctx.aspect_ratio()/+ctx.aspect_ratio()
    // Min - max y = -1/1
    glm::vec3 frontDetection = (glm::normalize(m_direction) / 2.f) + centeredCoord;

    glm::vec3 rightDetection = (glm::normalize(glm::rotate(m_direction, glm::radians(45.f), {0, 1, 0})) / 2.f) + centeredCoord;

    // Y detection
    avoidUpWall(*this, frontDetection, rightDetection, radius);
    avoidDownWall(*this, frontDetection, rightDetection, radius);

    // X detection
    avoidLeftWall(*this, frontDetection, rightDetection, radius);
    avoidRightWall(*this, frontDetection, rightDetection, radius);

    // Z detection
    avoidFrontWall(*this, frontDetection, rightDetection, radius);
    avoidBackWall(*this, frontDetection, rightDetection, radius);
}

void Boid::rotateLeft()
{
    m_direction = glm::rotate(m_direction, glm::radians(-15.f), {0, 1, 0});
}

void Boid::rotateRight()
{
    m_direction = glm::vec3(glm::rotate(m_direction, glm::radians(15.f), {0, 1, 0}));
}