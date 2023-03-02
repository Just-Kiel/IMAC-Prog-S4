#include "boid.hpp"
#include <vector>
#include "detections/detections.hpp"
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
    // Add cohesion process
    // m_direction += this->cohesion(neighbors);

    // Add separation process
    m_direction += this->separation(neighbors) * speed;

    // Add alignment process
    m_direction += this->alignment(neighbors) * speed;

    // Limit the speed
    m_direction = glm::normalize(m_direction);

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
    // else
    // {
    //     total = glm::vec3(p6::rotated_by(p6::Angle(m_direction), glm::vec2(1., 0.)), 0.);
    // }
    return total;
}

glm::vec3 Boid::alignment(const std::vector<Boid>& neighbors)
{
    float distanceOfVision = 0.2f;

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

float Boid::distance(const Boid& anotherBoid)
{
    return glm::distance2(this->centeredCoord, anotherBoid.centeredCoord);
}

glm::vec3 Boid::cohesion(const std::vector<Boid>& neighbors)
{
    glm::vec3 averagePos = glm::vec3(0, 0, 0);
    for (auto& boid : neighbors)
    {
        if (&boid != this)
        {
            averagePos += boid.centeredCoord;
        }
    }
    averagePos = glm::vec3(averagePos.x / (neighbors.size() - 1), averagePos.y / (neighbors.size() - 1), averagePos.z / (neighbors.size() - 1));

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

void Boid::avoidWalls(p6::Context& ctx)
{
    // Avoid going outside of window
    // Look forward head at a set distance
    // return other direction if there is something

    // Min - max x = -ctx.aspect_ratio()/+ctx.aspect_ratio()
    // Min - max y = -1/1
    glm::vec3 frontDetection = (glm::normalize(m_direction) / 2.f) + centeredCoord;
    glm::vec3 rightDetection = (glm::normalize(glm::vec3(p6::rotated_by(p6::Angle(m_direction) + 45_degrees, glm::vec2(1., 0.)), 0.))) + centeredCoord;

    // Y detection
    avoidUpWall(*this, frontDetection, rightDetection);
    avoidDownWall(*this, frontDetection, rightDetection);

    // X detection
    avoidLeftWall(*this, frontDetection, rightDetection, ctx);
    avoidRightWall(*this, frontDetection, rightDetection, ctx);
}

void Boid::rotateLeft()
{
    m_direction = glm::vec3(p6::rotated_by(p6::Angle(m_direction) - 15_degrees, glm::vec2(1., 0.)), 0.);
}

void Boid::rotateRight()
{
    m_direction = glm::vec3(p6::rotated_by(p6::Angle(m_direction) + 15_degrees, glm::vec2(1., 0.)), 0.);
}