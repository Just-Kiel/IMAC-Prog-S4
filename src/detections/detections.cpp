#include "detections.hpp"

void avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec)
{
    if (frontDetec.y > 1)
    {
        if (rightDetec.y > 1)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec)
{
    if (frontDetec.y < -1)
    {
        if (rightDetec.y < -1)
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, p6::Context& ctx)
{
    if (frontDetec.x < -ctx.aspect_ratio())
    {
        if (rightDetec.x < -ctx.aspect_ratio())
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}

void avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, p6::Context& ctx)
{
    if (frontDetec.x > ctx.aspect_ratio())
    {
        if (rightDetec.x > ctx.aspect_ratio())
        {
            currentBoid.rotateLeft();
        }
        else
        {
            currentBoid.rotateRight();
        }
    }
}