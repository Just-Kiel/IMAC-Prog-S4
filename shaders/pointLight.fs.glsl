#version 330 core

in vec3 vPosition_vs;
in vec3 vPosition_world;
in vec3 vNormal_vs;
//in vec2 vTexCoords;
in vec4 vFragPosLightSpace;
in vec3 vColor;


const int NB_LIGHTS = 2;
uniform vec3 u_lightsPos[NB_LIGHTS];

uniform vec3 uKd;
uniform vec3 uKs;
float uShininess;
uniform vec3 uLightIntensity;

// Uniforms for shadow mapping
// Depth computed from light
uniform vec3 uLightDir_vs;
uniform sampler2D shadowMap;

out vec4 fFragColor;

vec3 blinnPhong(vec3 lPos){

    //normal
    vec3 N = normalize(vNormal_vs);

    //direction
    vec3 W = normalize(lPos - vPosition_world);

    //attenuation
    float d = distance(vPosition_world, lPos);
    vec3 L = (1/(uLightIntensity + (d * d)));

    //diffuse shading
    float dotW = max(dot(W, N), 0.0);

    //specular shading
    vec3 W0 = normalize(-vPosition_world);   
    vec3 halfVector = normalize((W0 + W) / 2.0);
    float dotH = max(dot(halfVector, N), 0.0);
    float specular= pow(dotH, uShininess);

    return L * (uKd * dotW + uKs * specular);

}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(vNormal_vs, uLightDir_vs)), 0.005);

    // // PCF
    // Handle point light
    float shadow = 0.0;
    vec3 normal = normalize(vNormal_vs);
    vec3 lightDir = normalize(u_lightsPos[0] - vPosition_vs);
    bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    closestDepth = texture(shadowMap, projCoords.xy).r;
    currentDepth = projCoords.z;
    if(currentDepth - bias > closestDepth)
        shadow = 1.0;
    else
        shadow = 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;



    return shadow;
}

void main() {
    // lighting
    vec3 ambient = vec3(0.2);
    vec3 lightRes = vec3(0.0);
    for(int i = 0; i < NB_LIGHTS; i++){
        lightRes += blinnPhong(u_lightsPos[i]);
    }	
    fFragColor = vec4(lightRes, 1.0);

    
    // calculate shadow
    float shadow = ShadowCalculation(vFragPosLightSpace);     
    //shadow = 0.0;  
    vec3 lighting = ((1.0 - shadow) * vColor *(ambient + lightRes));

    fFragColor = vec4(lighting, 1.0);
}