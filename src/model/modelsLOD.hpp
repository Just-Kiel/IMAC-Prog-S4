#pragma once
#include <vector>
#include "LOD/LOD.hpp"
#include "model/model.hpp"

class ModelsLOD {
private:
    std::vector<Model> m_models;

public:
    explicit ModelsLOD(const std::array<std::string, LOD_COUNT>& modelsPaths);

    void drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params);
};