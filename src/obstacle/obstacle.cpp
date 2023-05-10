#include "obstacle.hpp"
#include <vector>
#include "detections/detections.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "obstacle/obstacle.hpp"

Obstacle::Obstacle(glm::vec3 position, float radius)
    : m_position(position), m_radius(radius)
{
    m_direction = glm::vec3(1, 0, 0);
}

ModelParams Obstacle::computeParams() const
{
    return ModelParams{
        .center    = m_position,
        .scale     = m_radius,
        .direction = m_direction,
    };
}

p6::Radius Obstacle::getRadius() const
{
    return m_radius;
}

glm::vec3 Obstacle::getPosition() const
{
    return m_position;
}

void addObstacle(p6::MouseButton& button, std::vector<Obstacle>& allObstacles)
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
            allObstacles.emplace_back(glm::vec3{-button.position.x, button.position.y, 0}, 0.1f);
        }
    }
}

std::vector<ModelParams> computeObstaclesParams(const std::vector<Obstacle>& allObstacles, const glm::vec3& viewMatrixPosition)
{
    std::vector<ModelParams> paramsAllObstacles{allObstacles.size()};
    for (auto const& obstacle : allObstacles)
    {
        ModelParams params = obstacle.computeParams();
        params.lod         = updateLOD(viewMatrixPosition, params.center);
        paramsAllObstacles.emplace_back(params);
    }

    return paramsAllObstacles;
}

void obstacleImgui(std::vector<Obstacle>& allObstacles)
{
    if (ImGui::BeginMenu("Obstacles"))
    {
        if (ImGui::Button("Clear Obstacles"))
        {
            allObstacles.clear();
        }
        ImGui::EndMenu();
    }
}