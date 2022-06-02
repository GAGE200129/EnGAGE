#version 330 core

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outColor;

in vec3 FSPosition;
in vec3 FSNormal;
in vec2 FSTexCoord;

out vec4 out_Color;

uniform sampler2D uTextureSheet;

void main()
{
	vec4 color = texture(uTextureSheet, FSTexCoord);

	outPosition = FSPosition;
	outNormal = normalize(FSNormal);
	outColor = color;
}