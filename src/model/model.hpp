#pragma once
#include <string>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Model {
private:
    std::string            m_urlPath;
    std::vector<glm::vec3> m_outVertices{};
    std::vector<glm::vec2> m_outUvs{};
    std::vector<glm::vec3> m_outNormals{};

    GLuint m_vao;
    GLuint m_vboPositions;
    GLuint m_vboNormals;
    GLuint m_vboTexCoords;

public:
    Model() = default;
    Model(std::string urlPath);
    ~Model();

    void loadObj();
    void initModel();

    void drawModel(const p6::Shader& shader, glm::mat4& ProjMatrix, glm::mat4& view, glm::vec3& center, p6::Radius& radius, glm::vec3& direction);
};