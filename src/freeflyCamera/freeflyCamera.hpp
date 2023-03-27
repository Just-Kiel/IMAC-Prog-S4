#pragma once

#include <glm/glm.hpp>
#include "p6/p6.h"

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

public:
    FreeflyCamera();

    void computeDirectionVectors();
    void moveLeft(float t);
    void moveFront(float t);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};

void cameraKeyControls(const p6::Key& key, FreeflyCamera& camera);