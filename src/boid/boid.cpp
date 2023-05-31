#include "boid.hpp"
#include <vector>
#include "boid/boid.hpp"
#include "detections/detections.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"

Boid::Boid(glm::vec3 center, float radius)
    : m_centered_coord(center), m_radius(radius)
{
    // make random glm vec 3 normalized
    m_direction = glm::normalize(glm::vec3(p6::random::number(), p6::random::number(), p6::random::number())); // We know it not perfect but its ok :)
};

ModelParams Boid::computeParams() const
{
    return ModelParams{
        .center    = m_centered_coord,
        .scale     = m_radius,
        .direction = m_direction,
    };
}

void Boid::updateCenter(const float& speed, const Forces& forces, const std::vector<Boid>& neighbors)
{
    // Add cohesion process
    m_direction += this->cohesion(neighbors) * forces.cohesionForce * speed;

    // Add separation process
    m_direction += this->separation(neighbors) * speed * forces.separationForce;

    // Add alignment process
    m_direction += this->alignment(neighbors) * speed * forces.alignmentForce;

    // Limit the speed
    m_direction = glm::normalize(m_direction);

    m_centered_coord += m_direction * speed / 100.f;
}

glm::vec3 Boid::separation(const std::vector<Boid>& neighbors)
{
    float maxDistance = m_radius / 2.f;

    // Count of boids to close from current boid
    size_t    count = 0;
    glm::vec3 total = glm::vec3(0, 0, 0);

    // Check for every boids if its to close
    for (auto const& boid : neighbors)
    {
        if (&boid == this)
            continue;

        // Calculate distance
        float distance = this->distance(boid);

        if (distance > maxDistance)
            continue;

        // Difference in term of position not length
        glm::vec3 difference = this->m_centered_coord - boid.m_centered_coord;

        glm::vec3 move = glm::normalize(difference) / static_cast<float>(glm::vec3::length());

        total += move;
        count++;
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
    for (auto const& boid : neighbors)
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
    return glm::distance2(this->m_centered_coord, anotherBoid.m_centered_coord);
}

glm::vec3 Boid::cohesion(const std::vector<Boid>& neighbors)
{
    float distanceAttraction = m_radius * 5.f;

    // Count of boids to close from current boid
    size_t count = 0;

    glm::vec3 averagePos = glm::vec3(0, 0, 0);
    for (auto const& boid : neighbors)
    {
        if (&boid != this)
        {
            // Calculate distance
            float distance = this->distance(boid);

            if (distance < distanceAttraction)
            {
                averagePos += boid.m_centered_coord;
                count++;
            }
        }
    }

    if (count != 0)
    {
        averagePos /= count;
        averagePos = glm::normalize(averagePos);
    }

    glm::vec3 distancePos = averagePos - this->m_centered_coord;

    return distancePos;
}

void Boid::avoidObstacles(const std::vector<Obstacle>& allObs)
{
    // Avoid going in an obstacle
    // Look forward head at a set distance
    // return other direction if there is something
    for (auto const& obs : allObs)
    {
        float vecLengthCenters = glm::l2Norm(obs.getPosition(), m_centered_coord);
        if (vecLengthCenters < obs.getRadius().value)
        {
            m_direction = glm::vec3(p6::rotated_by(p6::Angle(m_direction) + 25_degrees, glm::vec2(1., 0.)), 0.);
        }
    }
}

bool Boid::avoidWalls(const float& radius)
{
    // Avoid going outside of window
    // Look forward head at a set distance
    // return other direction if there is something

    // Min - max x = -ctx.aspect_ratio()/+ctx.aspect_ratio()
    // Min - max y = -1/1
    glm::vec3 frontDetection = (glm::normalize(m_direction) * m_radius) + m_centered_coord;

    glm::vec3 rightDetection = (glm::normalize(glm::rotate(m_direction, glm::radians(45.f), {0, 1, 0})) * m_radius) + m_centered_coord;

    glm::vec3 upDetection = (glm::normalize(glm::rotate(m_direction, glm::radians(45.f), {1, 0, 0})) * m_radius) + m_centered_coord;

    glm::vec3 downDetection = (glm::normalize(glm::rotate(m_direction, glm::radians(-45.f), {1, 0, 0})) * m_radius) + m_centered_coord;

    // Y detection
    return avoidUpWall(*this, frontDetection, upDetection, radius) || avoidDownWall(*this, frontDetection, downDetection, radius) ||

           // X detection
           avoidLeftWall(*this, frontDetection, rightDetection, radius) || avoidRightWall(*this, frontDetection, rightDetection, radius) ||

           // Z detection
           avoidFrontWall(*this, frontDetection, rightDetection, radius) || avoidBackWall(*this, frontDetection, rightDetection, radius);
}

void Boid::rotateLeft()
{
    m_direction = glm::rotate(m_direction, glm::radians(-15.f), {0, 1, 0});
}

void Boid::rotateRight()
{
    m_direction = glm::rotate(m_direction, glm::radians(15.f), {0, 1, 0});
}

void Boid::rotateUp()
{
    m_direction = glm::rotate(m_direction, glm::radians(15.f), {1, 0, 0});
}

void Boid::rotateDown()
{
    m_direction = glm::rotate(m_direction, glm::radians(-15.f), {1, 0, 0});
}

std::vector<Boid> createBoids()
{
    size_t nbBoids = 50;

    // Boids init
    std::vector<Boid> allBoids;

    for (size_t i = 0; i < nbBoids; ++i)
    {
        allBoids.emplace_back(
            glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
            0.01f
        );
    }

    return allBoids;
}

void ImguiBoids(std::vector<Boid>& boids)
{
    size_t nbBoids = boids.size();
    float  radius  = nbBoids != 0 ? boids[0].radius() : 0.01f;

    size_t minNbBoids = 0;
    size_t maxNbBoids = 1000;

    if (ImGui::SliderScalar("Boids number", ImGuiDataType_U32, &nbBoids, &minNbBoids, &maxNbBoids, "%u", ImGuiSliderFlags_AlwaysClamp))
    {
        while (nbBoids > boids.size())
        {
            boids.emplace_back(
                glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
                radius
            );
        }

        while (nbBoids < boids.size())
        {
            boids.pop_back();
        }
    }

    if (ImGui::SliderFloat("Boids radius", &radius, 0.f, 1.f))
    {
        boids.clear();

        for (unsigned int i = 0; i < nbBoids; ++i)
        {
            boids.emplace_back(
                glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
                radius
            );
        }
    }
}

std::vector<ModelParams> computeBoidsParams(const std::vector<Boid>& allBoids, const glm::vec3& viewMatrixPosition)
{
    std::vector<ModelParams> paramsAllBoids{};
    for (auto const& boid : allBoids)
    {
        ModelParams params = boid.computeParams();
        params.lod         = updateLOD(viewMatrixPosition, params.center);
        paramsAllBoids.emplace_back(params);
    }

    return paramsAllBoids;
}