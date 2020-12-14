#version 450 core // Minimal GL version support expected from the GPU

struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int isActive;
};


uniform LightSource lightSource[2];
uniform int nb_light;


struct Material {
    vec3 albedo;
    float shininess;
};

uniform Material material;



in vec3 fPosition; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)
in vec3 fPositionWorldSpace;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 colorResponse; // Shader output: the color response attached to this fragment


uniform mat4 projectionMat, modelViewMat, normalMat;

uniform sampler2D shadowMap[2];
uniform mat4 depthMVP[2];

float pi = 3.1415927;

float ShadowCalculation(vec4 fragPosLightSpace, unsigned int i)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap[i], projCoords.xy).r; 
    float currentDepth = projCoords.z;

    //Calculer bias pour éviter Shadow acne
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(lightSource[i].position - fPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    
    vec2 texelSize = 1.0 / textureSize(shadowMap[i], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap[i], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;


    //shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  


void main() {
    vec3 n = normalize(fNormal);

    // Linear barycentric interpolation does not preserve unit vectors
    vec3 wo = normalize (-fPosition); // unit vector pointing to the camera
    vec3 radiance = vec3(0,0,0);

    for(unsigned int i = 0; i < nb_light; i++){
        if( dot( n , wo ) >= 0.0 ) {
        {
            if( lightSource[i].isActive == 1 ) { // WE ONLY CONSIDER LIGHTS THAT ARE SWITCHED ON
                     {
                        vec3 wi = normalize ( vec3((modelViewMat * vec4(lightSource[i].position,1)).xyz) - fPosition ); // unit vector pointing to the light source (change if you use several light sources!!!)
                        if( dot( wi , n ) >= 0.0 ) { // WE ONLY CONSIDER LIGHTS THAT ARE ON THE RIGHT HEMISPHERE (side of the tangent plane)
                            vec3 wh = normalize( wi + wo ); // half vector (if wi changes, wo should change as well)
                            
                            float shadow = ShadowCalculation(depthMVP[i] * vec4(fPositionWorldSpace,1.0), i);

                            vec3 Li =(1.0 - shadow) * lightSource[i].color * lightSource[i].intensity;

                            radiance = radiance + 
                                    Li // light color
                                    * material.albedo
                                    * ( max(dot(n,wi),0.0) + pow(max(dot(n,wh),0.0),material.shininess) )
                                    ;
                        }
                    }
                }
            }
        }
    }
    

    colorResponse = vec4 (radiance, 1.0); // Building an RGBA value from an RGB one.

}




