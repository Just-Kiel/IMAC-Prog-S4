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

void addObstacle(const p6::Context& ctx, std::vector<Obstacle>& allObstacles, const glm::mat4& viewMatrix, const glm::mat4& projMatrix, const float& distanceToObstacle)
{
    bool OnOtherObstacle = false;

    if (ctx.mouse_button_is_pressed(p6::Button::Right))
    {
        glm::vec2 mousePos = ctx.mouse();
        mousePos.x *= ctx.inverse_aspect_ratio();

        glm::mat4 invVP     = glm::inverse(projMatrix * viewMatrix);
        glm::vec4 screenPos = glm::vec4(mousePos, distanceToObstacle, 1.0f);
        glm::vec4 worldPos  = invVP * screenPos;

        glm::vec3 worldPos3D = glm::vec3(worldPos) / worldPos.w;

        for (auto& obstacle : allObstacles)
        {
            float dist = glm::distance(obstacle.getPosition(), worldPos3D);
            if (dist <= 3 * obstacle.getRadius().value)
            {
                OnOtherObstacle = true;
            }
        }
        if (!OnOtherObstacle)
        {
            allObstacles.emplace_back(worldPos3D, 0.1f);
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

void obstacleImgui(std::vector<Obstacle>& allObstacles, float& distanceToObstacle)
{
    if (ImGui::BeginMenu("Obstacles"))
    {
        if (ImGui::Button("Clear Obstacles"))
        {
            allObstacles.clear();
        }

        ImGui::SliderFloat("Distance to Obstacle", &distanceToObstacle, -0.9f, 0.9f);
        ImGui::EndMenu();
    }
}