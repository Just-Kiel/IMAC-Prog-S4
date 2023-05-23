#version 330

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;
layout(location = 3) in vec3 aVertexColor;


layout(location = 4) in vec3 aModelCenter;
layout(location = 5) in float aModelScale;
layout(location = 6) in vec3 aModelDirection;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uLightSpaceMatrix;

out vec3 vPosition_vs;
out vec3 vPosition_world;
out vec3 vNormal_vs;
out vec2 vTexCoords;
out vec4 vFragPosLightSpace;
out vec3 vColor;

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

void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    // Calcul de la matrice modèle
    mat4 model = computeModelMatrix();
    // model = mat4(1.0f);

    // Calcul des matrices utiles
    mat4 uMVMatrix = uViewMatrix * model;
    mat4 uNormalMatrix = transpose(inverse(uMVMatrix));
    mat4 uMVPMatrix = uProjectionMatrix * uMVMatrix;

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