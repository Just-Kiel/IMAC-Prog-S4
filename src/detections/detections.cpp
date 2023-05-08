#include "detections.hpp"

bool avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& upDetection, const float& radius)
{
    if (frontDetec.y > radius)
    {
        if (upDetection.y > radius)
        {
            currentBoid.rotateDown();
        }
        else
        {
            currentBoid.rotateUp();
        }
        return true;
    }
    return false;
}

bool avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& downDetection, const float& radius)
{
    if (frontDetec.y < -radius)
    {
        if (downDetection.y < -radius)
        {
            currentBoid.rotateUp();
        }
        else
        {
            currentBoid.rotateDown();
        }
        return true;
    }
    return false;
}

bool avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
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
        return true;
    }
    return false;
}

bool avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
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
        return true;
    }
    return false;
}

bool avoidFrontWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
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
        return true;
    }
    return false;
}

bool avoidBackWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius)
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
        return true;
    }
    return false;
}