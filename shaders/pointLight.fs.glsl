#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
//in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

out vec4 fFragColor;

vec3 blinnPhong(){
    float d = distance(vPosition_vs, uLightPos_vs);
    
    vec3 N = normalize(vNormal_vs);
    vec3 L = (uLightIntensity / (d * d));
    vec3 W = normalize(uLightPos_vs - vPosition_vs);
    vec3 W0 = normalize(-vPosition_vs);
    
    vec3 halfVector = normalize((W0 + W) / 2.0);

    float dotW = max(dot(W, N), 0.0);
    float dotH = max(dot(halfVector, N), 0.0);
    float specular= pow(dotH, uShininess);

    return L * (uKd * dotW + uKs * specular);
}

void main() {
    fFragColor = vec4(blinnPhong(), 1.0);
}