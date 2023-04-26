#include "model/model.hpp"
#include <fstream>
#include <string>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

void computeDirectionVectors(glm::vec3& frontAxis, glm::vec3& leftAxis, glm::vec3& upAxis, const glm::vec3& direction)
{
    frontAxis = glm::normalize(direction);
    leftAxis  = glm::normalize(glm::cross(frontAxis, glm::vec3(0, 1, 0)));
    upAxis    = glm::normalize(glm::cross(leftAxis, frontAxis));
}

Model::Model(std::string urlPath)
    : m_urlPath(std::move(urlPath))
{
    loadObj();
    initModel();
}

void Model::loadObj()
{
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;
    std::vector<glm::vec3>    tempVertices;
    std::vector<glm::vec2>    tempUvs;
    std::vector<glm::vec3>    tempNormals;

    std::ifstream file;
    file.open(p6::make_absolute_path(m_urlPath), std::ios_base::in);
    if (!file.is_open())
    {
        std::cout << "Impossible to open the file !\n";
    }

    while (!file.eof())
    {
        std::string lineHeader;

        file >> lineHeader;

        if (lineHeader == "v")
        {
            glm::vec3 vertex(0.0f, 0.0f, 0.0f);
            // Parse line with 3 floats for vertex
            file.ignore(1);
            file >> vertex.x;
            file >> vertex.y;
            file >> vertex.z;

            tempVertices.push_back(vertex);
        }
        else if (lineHeader == "vt")
        {
            glm::vec2 uv;
            file.ignore(1);
            file >> uv.x;
            file >> uv.y;
            tempUvs.push_back(uv);
        }
        else if (lineHeader == "vn")
        {
            glm::vec3 normal;
            file.ignore(1);
            file >> normal.x;
            file >> normal.y;
            file >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (lineHeader == "f")
        {
            unsigned int vertexIndex[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];

            file.ignore(1);
            file >> vertexIndex[0];
            file.ignore(1);
            file >> uvIndex[0];
            file.ignore(1);
            file >> normalIndex[0];

            file >> vertexIndex[1];
            file.ignore(1);
            file >> uvIndex[1];
            file.ignore(1);
            file >> normalIndex[1];

            file >> vertexIndex[2];
            file.ignore(1);
            file >> uvIndex[2];
            file.ignore(1);
            file >> normalIndex[2];

            for (unsigned int element : vertexIndex)
            {
                vertexIndices.push_back(element);
            }
            for (unsigned int element : uvIndex)
            {
                uvIndices.push_back(element);
            }
            for (unsigned int element : normalIndex)
            {
                normalIndices.push_back(element);
            }
        }
        else
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // For each vertex of each triangle
    for (unsigned int vertexIndex : vertexIndices)
    {
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        m_outVertices.push_back(vertex);
    }

    for (unsigned int uvIndex : uvIndices)
    {
        glm::vec2 uv = tempUvs[uvIndex - 1];
        m_outUvs.push_back(uv);
    }

    for (unsigned int normalIndex : normalIndices)
    {
        glm::vec3 normal = tempNormals[normalIndex - 1];
        m_outNormals.push_back(normal);
    }
}

void Model::initModel()
{
    glBindBuffer(GL_ARRAY_BUFFER, *m_vboPositions);
    glBufferData(GL_ARRAY_BUFFER, m_outVertices.size() * sizeof(glm::vec3), m_outVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, *m_vboNormals);
    glBufferData(GL_ARRAY_BUFFER, m_outNormals.size() * sizeof(glm::vec3), m_outNormals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, *m_vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, m_outUvs.size() * sizeof(glm::vec2), m_outUvs.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(*m_vao);

    const GLuint VERTEX_ATTR_POSITION   = 0;
    const GLuint VERTEX_ATTR_NORMAL     = 1;
    const GLuint VERTEX_ATTR_TEX_COORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEX_COORDS);

    // Specification
    glBindBuffer(GL_ARRAY_BUFFER, *m_vboPositions);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, *m_vboNormals);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glBindBuffer(GL_ARRAY_BUFFER, *m_vboTexCoords);
    glVertexAttribPointer(VERTEX_ATTR_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::drawModel(const p6::Shader& shader, const glm::mat4& ProjMatrix, const glm::mat4& view, const ModelParams& params)
{
    glm::mat4 MVMatrix = view;

    // Translate the model to the position
    MVMatrix = glm::translate(MVMatrix, params.center);

    // Rotate the model to the correct direction
    glm::vec3 frontAxis, leftAxis, upAxis;

    computeDirectionVectors(frontAxis, leftAxis, upAxis, params.direction);

    // construct matrix from axes
    glm::mat4 rotationMatrix = glm::mat4(
        glm::vec4(leftAxis, 0.0f),
        glm::vec4(upAxis, 0.0f),
        glm::vec4(frontAxis, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    MVMatrix = MVMatrix * rotationMatrix;

    // Scale the model to the correct radius
    MVMatrix = glm::scale(MVMatrix, glm::vec3{params.scale});

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glm::mat4 MVPProduct = ProjMatrix * MVMatrix;

    // Set uniforms
    shader.set("uMVPMatrix", MVPProduct);
    shader.set("uMVMatrix", MVMatrix);
    shader.set("uNormalMatrix", NormalMatrix);

    glBindVertexArray(*m_vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_outVertices.size()));
    glBindVertexArray(0);
}