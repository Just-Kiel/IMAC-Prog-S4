#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aVertexColor;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform mat4 uLightSpaceMatrix;
uniform mat4 model;

out vec3 vPosition_vs;
out vec3 vPosition_world;
out vec3 vNormal_vs;
out vec2 vTexCoords;
out vec4 vFragPosLightSpace;
out vec3 vColor;

void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vPosition_world = vec3(model * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;
    vFragPosLightSpace = uLightSpaceMatrix * model * vertexPosition;
    vColor = aVertexColor;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}