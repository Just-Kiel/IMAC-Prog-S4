#pragma once
#include <array>
#include <string>
#include "model/modelsLOD.hpp"
#include "opengl/framebufferOpenGl.hpp"
#include "p6/p6.h"

class ShadowMapping {
private:
    p6::Shader  m_shader;
    Framebuffer m_framebuffer{0, 0};

public:
    bool m_displayShadow{true};

public:
    explicit ShadowMapping(std::array<std::string, 2> shaderPaths, const int& width, const int& height);

    void render(ModelsLOD& casterModel, const std::vector<ModelParams>& castersParams, const glm::mat4& ProjMatrix, const glm::mat4& lightSpaceMatrix);
    void render(Model& casterModel, const std::vector<ModelParams>& castersParams, const VBO& vboInstancing, const glm::mat4& ProjMatrix, const glm::mat4& lightSpaceMatrix);

    void activateTexture(const p6::Shader& shader) const;
};