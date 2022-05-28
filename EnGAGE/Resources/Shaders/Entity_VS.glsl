#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 FSPosition;
out vec3 FSNormal;
out vec2 FSTexCoord;

uniform mat4 uProjView;
uniform mat4 uModel;

void main()
{
	vec4 position = uModel * vec4(inPos, 1); 
	FSPosition = position.xyz;
	FSNormal = (mat3(uModel) * inNormal).xyz;
	FSTexCoord = inTexCoord;
	gl_Position = uProjView * position;
}