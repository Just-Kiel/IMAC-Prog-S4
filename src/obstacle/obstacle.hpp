#pragma once
#include "glm/fwd.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "p6/p6.h"

class Obstacle {
private:
    glm::vec3 m_position;

    glm::vec3 m_direction;

    float m_radius;

public:
    Obstacle(glm::vec3 position, float radius);

    ModelParams computeParams() const;

    void updateLOD(glm::mat4& view);

    p6::Radius getRadius() const;

    glm::vec3 getPosition() const;
};

void                     addObstacle(p6::MouseButton& button, std::vector<Obstacle>& allObstacles);
std::vector<ModelParams> computeObstaclesParams(const std::vector<Obstacle>& allObstacles, const glm::vec3& viewMatrixPosition);
void                     obstacleImgui(std::vector<Obstacle>& allObstacles);