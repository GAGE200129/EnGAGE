#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 FSTexCoord;
out vec3 FSNormal;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;

void main()
{
	FSTexCoord = inTexCoord;
	FSNormal = normalize(inNormal);
	gl_Position = uProj * uView * uModel * vec4(inPos, 1);
}