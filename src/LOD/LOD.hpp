#pragma once
#include <cstdint>
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

enum LOD : int {
    LOD_LOW,
    LOD_MEDIUM,
    LOD_HIGH,

    LOD_COUNT
};

LOD updateLOD(glm::vec3 cameraPosition, glm::vec3 modelPosition);