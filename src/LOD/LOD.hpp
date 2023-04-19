#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

enum LOD {
    LOD_LOW,
    LOD_MEDIUM,
    LOD_HIGH,

    LOD_COUNT
};

LOD updateLOD(glm::vec3 cameraPosition, glm::vec3 modelPosition);