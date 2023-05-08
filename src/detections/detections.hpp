#include "boid/boid.hpp"

bool avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& upDetection, const float& radius);
bool avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& downDetection, const float& radius);
bool avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
bool avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
bool avoidFrontWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);
bool avoidBackWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, const float& radius);