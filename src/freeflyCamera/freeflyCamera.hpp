#pragma once

#include <glm/glm.hpp>
#include "p6/p6.h"

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;

    glm::vec3 m_FrontVector{};
    glm::vec3 m_LeftVector{};
    glm::vec3 m_UpVector{};

public:
    FreeflyCamera();

    void computeDirectionVectors();
    void moveLeft(float t);
    void moveFront(float t);
    void moveUp(float t);
    void moveDown(float t);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFrontVector() const;

    void reset();
    void constraintCamera(float radius);
};

void cameraKeyControls(const p6::Context& ctx, FreeflyCamera& camera, float deltaTime, float radiusCell);
void cameraImgui(FreeflyCamera& camera);