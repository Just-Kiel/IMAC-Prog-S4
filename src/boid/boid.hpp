#pragma once
#include <array>
#include <optional>
#include <vector>
#include "LOD/LOD.hpp"
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

    float m_radius{1.f};

    glm::vec3 m_direction{1.f, 0.f, 0.f};

public:
    Boid(glm::vec3 center, float radius);

    inline float radius() const { return m_radius; }

    ModelParams computeParams() const;

    void updateCenter(const float& speed, const Forces& forces, const std::vector<Boid>& neighbors);

    glm::vec3 separation(const std::vector<Boid>& neighbors);
    glm::vec3 alignment(const std::vector<Boid>& neighbors);
    glm::vec3 cohesion(const std::vector<Boid>& neighbors);

    float distance(const Boid& anotherBoid) const;

    void avoidObstacles(const std::vector<Obstacle>& allObstacles);

    void avoidWalls(const float& radius);

    void rotateLeft();

    void rotateRight();
};

void                     ImguiBoids(std::vector<Boid>& boids);
std::vector<ModelParams> computeBoidsParams(const std::vector<Boid>& allBoids, const glm::vec3& viewMatrixPosition);