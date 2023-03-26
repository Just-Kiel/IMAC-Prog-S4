#pragma once
#include <vector>
#include "model/model.hpp"

class ModelsLOD {
private:
    std::vector<Model> m_models;

public:
    ModelsLOD() = default;
    ModelsLOD(std::vector<std::string> modelsPaths);
    ~ModelsLOD() = default;

    void initModels();

    void drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params);
};