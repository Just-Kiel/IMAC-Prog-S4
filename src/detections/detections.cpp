#include "detections.hpp"

void avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.y > radius)
    {
        if (rightDetec.y > radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.y < -radius)
    {
        if (rightDetec.y < -radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.x < -radius)
    {
        if (rightDetec.x < -radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.x > radius)
    {
        if (rightDetec.x > radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidFrontWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.z < -radius)
    {
        if (rightDetec.z < -radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidBackWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
{
    if (frontDetec.z > radius)
    {
        if (rightDetec.z > radius)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}