#version 330 core

in vec2 FSTexCoord;
in vec3 FSNormal;

out vec4 out_Color;

uniform sampler2D uBaseColor;

const vec3 lightDirection = normalize(vec3(1, -1, 0));

void main()
{
	float diffuseFactor = max(0.1, dot(-lightDirection, FSNormal));
	out_Color = vec4(0.5, 0.5, 0.5, 1) * diffuseFactor;
}