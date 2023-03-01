#include "boid/boid.hpp"

void avoidUpWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec);
void avoidDownWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec);
void avoidLeftWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, p6::Context& ctx);
void avoidRightWall(Boid& currentBoid, const glm::vec3& frontDetec, const glm::vec3& rightDetec, p6::Context& ctx);