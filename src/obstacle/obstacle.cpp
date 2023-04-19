#include "obstacle.hpp"
#include <vector>
#include "detections/detections.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "obstacle/obstacle.hpp"

Obstacle::Obstacle(glm::vec3 position, float radius, ModelsLOD& model)
    : m_position(position), m_radius(radius), m_model(model)
{
    m_direction = glm::vec3(1, 0, 0);
}

void Obstacle::draw(const p6::Shader& shader, glm::mat4& projection, glm::mat4& view)
{
    updateLOD(view);

    m_model.drawModel(
        shader,
        projection,
        view,
        ModelParams{
            m_position,
            m_radius,
            m_direction,
            m_lod}
    );
}

// TODO free function
void Obstacle::updateLOD(glm::mat4& view)
{
    // extract position from glm mat4
    glm::vec3 position = glm::vec3(view[3]);

    // calculate distance between boid and camera
    float distance = glm::distance2(this->getPosition(), position);

    // if distance is less than 1, set LOD to LOD_HIGH
    if (distance < 5)
    {
        m_lod = LOD::LOD_HIGH;
    }
    else if (distance < 15)
    {
        m_lod = LOD::LOD_MEDIUM;
    }
    else
    {
        m_lod = LOD::LOD_LOW;
    }
}

p6::Radius Obstacle::getRadius() const
{
    return m_radius;
}

glm::vec3 Obstacle::getPosition() const
{
    return m_position;
}

void addObstacle(p6::MouseButton& button, std::vector<Obstacle>& allObstacles, ModelsLOD& modelObstacleLOD)
{
    bool OnOtherObstacle = false;
    if (button.button == p6::Button::Right)
    {
        for (auto& obstacle : allObstacles)
        {
            float dist = glm::distance(glm::vec2(obstacle.getPosition()), button.position);
            if (dist <= 4 * obstacle.getRadius().value)
            {
                OnOtherObstacle = true;
            }
        }
        if (!OnOtherObstacle)
        {
            // TODO(Olivia) change in function cell size / view matrix
            allObstacles.emplace_back(glm::vec3{-button.position.x, button.position.y, 0}, 0.1f, modelObstacleLOD);
        }
    }
}