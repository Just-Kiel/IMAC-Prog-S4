#pragma once
#include "opengl/textureOpenGl.hpp"
#include "opengl/vao.hpp"
#include "opengl/vbo.hpp"

void createSkybox(Texture& cubemapTexture, std::vector<img::Image>& facesImg, const VAO& skyboxVAO, const VBO& skyboxVBO);