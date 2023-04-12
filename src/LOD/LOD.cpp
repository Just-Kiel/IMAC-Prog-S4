#include "LOD.hpp"

LOD updateLOD(glm::vec3 cameraPosition, glm::vec3 modelPosition)
{
    // calculate distance between boid and camera
    float distance = glm::distance2(modelPosition, cameraPosition);

    // default LOD is LOD_LOW to avoid errors
    LOD lod = LOD::LOD_LOW;

    // if distance is less than 5, set LOD to LOD_HIGH
    if (distance < 5)
    {
        lod = LOD::LOD_HIGH;
    }
    else if (distance < 15)
    {
        lod = LOD::LOD_MEDIUM;
    }
    else
    {
        lod = LOD::LOD_LOW;
    }

    return lod;
}