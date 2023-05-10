#include "freeflyCamera.hpp"
#include "GLFW/glfw3.h"
#include "freeflyCamera/freeflyCamera.hpp"
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

void FreeflyCamera::moveUp(float t)
{
    m_Position += t * glm::vec3(0.f, 1.f, 0.f);
}

void FreeflyCamera::moveDown(float t)
{
    m_Position -= t * glm::vec3(0.f, 1.f, 0.f);
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

void FreeflyCamera::reset()
{
    m_Position = glm::vec3(0.f, 0.f, 0.f);
    m_Phi      = p6::PI;
    m_Theta    = 0.f;
    computeDirectionVectors();
}

void FreeflyCamera::constraintCamera(float radius)
{
    if (m_Position.y < -radius)
    {
        m_Position.y = -radius;
    }
    if (m_Position.y > radius)
    {
        m_Position.y = radius;
    }

    if (m_Position.x < -radius)
    {
        m_Position.x = -radius;
    }
    if (m_Position.x > radius)
    {
        m_Position.x = radius;
    }

    if (m_Position.z < -radius)
    {
        m_Position.z = -radius;
    }
    if (m_Position.z > radius)
    {
        m_Position.z = radius;
    }
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void cameraKeyControls(const p6::Context& ctx, FreeflyCamera& camera, float deltaTime, float radiusCell)
{
    if (ctx.key_is_pressed(GLFW_KEY_A) || ctx.key_is_pressed(GLFW_KEY_LEFT))
    {
        camera.moveLeft(deltaTime);
    }

    if (ctx.key_is_pressed(GLFW_KEY_D) || ctx.key_is_pressed(GLFW_KEY_RIGHT))
    {
        camera.moveLeft(-deltaTime);
    }

    if (ctx.key_is_pressed(GLFW_KEY_W) || ctx.key_is_pressed(GLFW_KEY_UP))
    {
        camera.moveFront(deltaTime);
    }

    if (ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_DOWN))
    {
        camera.moveFront(-deltaTime);
    }

    if (ctx.key_is_pressed(GLFW_KEY_SPACE))
    {
        camera.moveUp(deltaTime);
    }

    if (ctx.shift())
    {
        camera.moveDown(deltaTime);
    }

    camera.constraintCamera(radiusCell);
}

void cameraImgui(FreeflyCamera& camera)
{
    if (ImGui::BeginMenu("Camera"))
    {
        if (ImGui::Button("Go back to the origin"))
        {
            camera.reset();
        }
        ImGui::EndMenu();
    }
}