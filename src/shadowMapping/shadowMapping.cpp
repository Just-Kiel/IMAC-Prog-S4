#include "shadowMapping.hpp"
#include <vector>
#include "model/model.hpp"
#include "model/modelsLOD.hpp"
#include "opengl/framebufferOpenGl.hpp"

ShadowMapping::ShadowMapping(std::array<std::string, 2> shaderPaths, const int& width, const int& height)
    : m_shader(p6::load_shader(shaderPaths[0], shaderPaths[1])), m_framebuffer(Framebuffer(width, height))
{
}

void ShadowMapping::render(ModelsLOD& casterModel, const std::vector<ModelParams>& castersParams, const glm::mat4& ProjMatrix, const glm::mat4& lightSpaceMatrix)
{
    if (!m_displayShadow)
        return;

    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, *m_framebuffer);
    glViewport(0, 0, m_framebuffer.getWidth(), m_framebuffer.getHeight()); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClear(GL_DEPTH_BUFFER_BIT);

    m_shader.use();
    m_shader.set("uLightSpaceMatrix", lightSpaceMatrix);

    // We only draw the casters
    glCullFace(GL_FRONT);
    for (const auto& castersParam : castersParams)
    {
        casterModel.drawModel(m_shader, ProjMatrix, lightSpaceMatrix, castersParam);
    }

    // Render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK); // don't forget to reset original culling face

    glViewport(0, 0, m_framebuffer.getWidth() / 2, m_framebuffer.getHeight() / 2); // Render on the whole framebuffer, complete from the lower left corner to the upper right
}

void ShadowMapping::activateTexture(const p6::Shader& shader) const
{
    if (!m_displayShadow)
        return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_framebuffer.getTextureId());
    shader.set("shadowMap", 0);
}