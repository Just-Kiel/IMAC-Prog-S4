#include "model/modelsLOD.hpp"

ModelsLOD::ModelsLOD(std::vector<std::string> modelsPaths)
{
    for (auto& modelsPath : modelsPaths)
    {
        m_models.emplace_back(modelsPath);
    }
}

void ModelsLOD::initModels()
{
    for (auto& model : m_models)
    {
        model.loadObj();
        model.initModel();
    }
}

void ModelsLOD::drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params)
{
    m_models[params.lod].drawModel(shader, ProjMatrix, view, params);
}