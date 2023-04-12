#include "model/modelsLOD.hpp"
#include "LOD/LOD.hpp"

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
    // extract position of camera from glm mat4
    auto position = glm::vec3(view[3]);

    // update LOD corresponding to the distance between the camera and the model
    LOD lod = updateLOD(position, params.center);

    // draw model corresponding to the LOD
    m_models[lod]
        .drawModel(shader, ProjMatrix, view, params);
}