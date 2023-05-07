#include "freeflyCamera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "p6/p6.h"

FreeflyCamera::FreeflyCamera()
    : m_Position(0.f, 0.f, 0.f), m_Phi(p6::PI), m_Theta(0.f)
{
    computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(
        cos(glm::radians(m_Theta)) * sin(glm::radians(m_Phi)),
        sin(glm::radians(m_Theta)),
        cos(glm::radians(m_Theta)) * cos(glm::radians(m_Phi))
    );
    m_LeftVector = glm::vec3(
        sin(glm::radians(m_Phi) + glm::radians(90.f)),
        0.f,
        cos(glm::radians(m_Phi) + glm::radians(90.f))
    );
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
}

void FreeflyCamera::moveFront(float t)
{
    m_Position += t * m_FrontVector;
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += degrees;
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_Theta += degrees;
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void cameraKeyControls(const p6::Key& key, FreeflyCamera& camera)
{
    if (key.physical == GLFW_KEY_A || key.physical == GLFW_KEY_LEFT)
    {
        camera.moveLeft(0.5f);
    }
    else if (key.physical == GLFW_KEY_D || key.physical == GLFW_KEY_RIGHT)
    {
        camera.moveLeft(-0.5f);
    }
    else if (key.physical == GLFW_KEY_W || key.physical == GLFW_KEY_UP)
    {
        camera.moveFront(0.5f);
    }
    else if (key.physical == GLFW_KEY_S || key.physical == GLFW_KEY_DOWN)
    {
        camera.moveFront(-0.5f);
    }
}