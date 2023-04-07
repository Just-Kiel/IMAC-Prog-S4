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

    ModelsLOD& m_model;

    LOD m_lod = LOD::LOD_LOW;

public:
    Obstacle(glm::vec3 position, float radius, ModelsLOD& model);

    ~Obstacle() = default;

    void draw(const p6::Shader& shader, glm::mat4& projection, glm::mat4& view);

    void updateLOD(glm::mat4& view);

    p6::Radius getRadius() const;

    glm::vec3 getPosition() const;
};