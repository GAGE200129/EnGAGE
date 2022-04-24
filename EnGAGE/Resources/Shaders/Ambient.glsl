#version 330 core

out vec4 OutColor;

in vec2 FSTexCoord;

uniform sampler2D gPositionTex;
uniform sampler2D gNormalTex;
uniform sampler2D gColorTex;

uniform vec3 uAmbient;

void main()
{
    // retrieve data from G-buffer
    vec3 color = texture(gColorTex, FSTexCoord).rgb;

    OutColor = vec4(color * uAmbient, 1);
}