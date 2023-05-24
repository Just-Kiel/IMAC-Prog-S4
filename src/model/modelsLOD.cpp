#include "model/modelsLOD.hpp"
#include <numeric>
#include "LOD/LOD.hpp"

ModelsLOD::ModelsLOD(const std::array<std::string, LOD_COUNT>& modelsPaths)
{
    for (const auto& path : modelsPaths)
    {
        m_models.emplace_back(path);
    }
}

void ModelsLOD::drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params)
{
    // draw model corresponding to the LOD
    m_models[params.lod]
        .drawModel(shader, ProjMatrix, view, params);
}