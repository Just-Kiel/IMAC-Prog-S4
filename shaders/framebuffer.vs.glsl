#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uLightSpaceMatrix;
uniform mat4 model;

layout(location = 4) in vec3 aModelCenter;
layout(location = 5) in float aModelScale;
layout(location = 6) in vec3 aModelDirection;
uniform bool uIsInstanced;

mat4 computeModelMatrix()
{
    // get model parameters
    vec3 center = aModelCenter;
    float scale = aModelScale;
    vec3 direction = aModelDirection;

    // compute model matrix    
    vec3 frontAxis = normalize(direction);
    vec3 leftAxis  = cross(frontAxis, vec3(0, 1, 0));
    vec3 upAxis    = cross(leftAxis, frontAxis);

    mat4 rotationMatrix = mat4(
        vec4(leftAxis, 0.0f),
        vec4(upAxis, 0.0f),
        vec4(frontAxis, 0.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    mat4 translationMatrix = mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(center, 1.0)
    );

    mat4 scaleMatrix = mat4(
        vec4(scale, 0.0, 0.0, 0.0),
        vec4(0.0, scale, 0.0, 0.0),
        vec4(0.0, 0.0, scale, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    mat4 modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

    return modelMatrix;
}

void main()
{
    mat4 modelMatrix = model;
    if(uIsInstanced)
    {
        modelMatrix = computeModelMatrix();
    }
    gl_Position =  uLightSpaceMatrix * modelMatrix * vec4(aPos, 1.0);
}  