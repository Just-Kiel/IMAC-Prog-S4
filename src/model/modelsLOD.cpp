#include "model/modelsLOD.hpp"

ModelsLOD::ModelsLOD(std::vector<std::string> modelsPaths)
    : m_models(modelsPaths.size())
{
    for (size_t i = 0; i < modelsPaths.size(); i++)
    {
        m_models[i] = Model(modelsPaths[i]);
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