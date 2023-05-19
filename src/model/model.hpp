#pragma once
#include <string>
#include <vector>
#include "LOD/LOD.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "opengl/vao.hpp"
#include "opengl/vbo.hpp"
#include "p6/p6.h"

// Struct for model params
struct ModelParams {
    glm::vec3 center;
    float     scale;
    glm::vec3 direction;
    LOD       lod;
};

class Model {
private:
    std::string            m_urlPath;
    std::vector<glm::vec3> m_outVertices{};
    std::vector<glm::vec2> m_outUvs{};
    std::vector<glm::vec3> m_outNormals{};
    std::vector<glm::vec3> m_outColors{};

    VAO m_vao{};
    VBO m_vboPositions{};
    VBO m_vboNormals{};
    VBO m_vboTexCoords{};
    VBO m_vboColors{};

public:
    explicit Model(std::string urlPath);

    void loadObj();
    void initModel();

    void drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params);
};