#version 330 core

out vec4 OutColor;

in vec2 FSTexCoord;

uniform sampler2D gPositionTex;
uniform sampler2D gNormalTex;
uniform sampler2D gColorTex;

uniform vec3 uDirection;
uniform vec3 uColor;
uniform float uIntensity;
uniform vec3 uCamPos;

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
    }
     

    OutColor = vec4(totalLight, 1) * uIntensity;
}