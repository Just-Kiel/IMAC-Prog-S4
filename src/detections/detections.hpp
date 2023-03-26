#include "boid/boid.hpp"

void avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
void avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
void avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
void avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
void avoidFrontWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
void avoidBackWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);