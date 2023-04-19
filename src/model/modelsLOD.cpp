#include "model/modelsLOD.hpp"
#include <vcruntime.h>
#include <numeric>
#include "LOD/LOD.hpp"

ModelsLOD::ModelsLOD(std::array<std::string, LOD_COUNT> modelsPaths)
{
    for (int i = 0; i < LOD_COUNT; i++)
    {
        m_models.emplace_back(modelsPaths[i]);
    }
}

void ModelsLOD::initModels()
{
    for (auto& model : m_models)
    {
        model.loadObj();   // TODO do this in Model constructor
        model.initModel(); // TODO do this in Model constructor
    }
}

void ModelsLOD::drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params)
{
    // draw model corresponding to the LOD
    m_models[params.lod]
        .drawModel(shader, ProjMatrix, view, params);
}