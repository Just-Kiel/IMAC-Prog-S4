#pragma once
#include <array>
#include <optional>
#include <vector>
#include "LOD.hpp"
#include "forces/forces.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "obstacle/obstacle.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_centered_coord{};

    float m_radius;

    glm::vec3 m_direction;

    Forces m_forces{};

    // TODO(Aurore): Instead draw with parameter and separate LOD from BOID
    ModelsLOD& m_model;

    LOD m_lod = LOD::LOD_LOW;

public:
    Boid(glm::vec3 center, float radius, ModelsLOD& model);

    ~Boid() = default;

    inline float radius() const { return m_radius; }

    void setForces(Forces forces);

    //  TODO rendering params and model
    void draw(const p6::Shader& shader, glm::mat4& projection, glm::mat4& view);

    void updateLOD(glm::mat4& view);

    void updateCenter(float speed, const std::vector<Boid>& neighbors);

    glm::vec3 separation(const std::vector<Boid>& neighbors);
    glm::vec3 alignment(const std::vector<Boid>& neighbors);
    glm::vec3 cohesion(const std::vector<Boid>& neighbors);

    float distance(const Boid& anotherBoid) const;

    void avoidObstacles(const std::vector<Obstacle>& allObstacles);

    void avoidWalls(const float& radius);

    void rotateLeft();

    void rotateRight();
};