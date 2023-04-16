#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
//in vec2 vTexCoords;


const int NB_LIGHTS = 2;
uniform vec3 u_lightsPos[NB_LIGHTS];

uniform vec3 uKd;
uniform vec3 uKs;
float uShininess;
uniform vec3 uLightIntensity;

out vec4 fFragColor;

vec3 blinnPhong(vec3 lPos){

    //normal
    vec3 N = normalize(vNormal_vs);

    //direction
    vec3 W = normalize(lPos - vPosition_vs);

    //attenuation
    float d = distance(vPosition_vs, lPos);
    vec3 L = (uLightIntensity / (d * d));

    //diffuse shading
    float dotW = max(dot(W, N), 0.0);

    //specular shading
    vec3 W0 = normalize(-vPosition_vs);   
    vec3 halfVector = normalize((W0 + W) / 2.0);
    float dotH = max(dot(halfVector, N), 0.0);
    float specular= pow(dotH, uShininess);

    return L * (uKd * dotW + uKs * specular);

}

void main() {
    vec3 lightRes = vec3(0.0);
    for(int i = 0; i < NB_LIGHTS; i++){
        lightRes += blinnPhong(u_lightsPos[i]);
    }	
    fFragColor = vec4(lightRes, 1.0);
}