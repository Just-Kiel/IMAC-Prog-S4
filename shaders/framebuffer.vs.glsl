#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uLightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position =  uLightSpaceMatrix * model * vec4(aPos, 1.0);
}  