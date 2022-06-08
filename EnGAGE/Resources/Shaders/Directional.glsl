#version 330 core

out vec4 OutColor;

in vec2 FSTexCoord;

uniform sampler2D gPositionTex;
uniform sampler2D gNormalTex;
uniform sampler2D gColorTex;
uniform sampler2D uDepthMap;

uniform vec3 uDirection;
uniform vec3 uColor;
uniform float uIntensity;
uniform vec3 uCamPos;
uniform mat4 uLightProjView;

float calShadowMap(vec3 fragPos, vec3 normal)
{
    vec4 fragPosLightSpace = uLightProjView *  vec4(fragPos, 1);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 
    float closestDepth = texture(uDepthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;  

    float shadow = 0.0;
    float bias = max(0.05 * (1.0 - dot(normal, uDirection)), 0.005);
    vec2 texelSize = 1.0 / textureSize(uDepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uDepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    vec3 fragPos = texture(gPositionTex, FSTexCoord).rgb;
    vec3 normal = texture(gNormalTex, FSTexCoord).rgb;
    vec3 color = texture(gColorTex, FSTexCoord).rgb;

    vec3 totalLight = vec3(0, 0, 0);

    vec3 lightDir = -normalize(uDirection);
    vec3 viewDir = normalize(uCamPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    if(diffuseFactor > 0.0)
    {
        totalLight += diffuseFactor * color * uColor;
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        totalLight += 1.0 * spec * uColor;
        totalLight *= (1.0 - calShadowMap(fragPos, normal));
    }
     

    OutColor = vec4(totalLight, 1) * uIntensity;
}