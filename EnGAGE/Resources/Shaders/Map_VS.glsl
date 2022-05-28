#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

out vec3 FSPosition;
out vec3 FSNormal;
out vec2 FSTexCoord;

uniform mat4 uProjView;

void main()
{
	vec4 position = vec4(inPos, 1); 
	FSPosition = position.xyz;
	FSNormal = inNormal;
	FSTexCoord = inTexCoord;
	gl_Position = uProjView * position;
}