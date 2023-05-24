#include "skybox.hpp"
#include <array>

const std::array skyboxVertices = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

void createSkybox(Texture& cubemapTexture, std::vector<img::Image>& facesImg, const VAO& skyboxVAO, const VBO& skyboxVBO)
{
    std::vector<std::string> faces = {
        "skybox/right.png",
        "skybox/left.png",
        "skybox/top.png",
        "skybox/bottom.png",
        "skybox/front.png",
        "skybox/back.png",
    };

    for (auto& face : faces)
    {
        facesImg.emplace_back(p6::load_image_buffer("assets/textures/" + face, false));
    }

    cubemapTexture.cubemap(facesImg, GL_RGBA, GL_UNSIGNED_BYTE);

    glBindVertexArray(*skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

void skyboxImgui(float& exposure, const p6::Shader& skyboxShader)
{
    if (ImGui::BeginMenu("Skybox"))
    {
        // Slider for exposure
        if (ImGui::SliderFloat("Exposure", &exposure, 0.1f, 6.f))
        {
            skyboxShader.set("exposure", exposure);
        }
        ImGui::EndMenu();
    }
}