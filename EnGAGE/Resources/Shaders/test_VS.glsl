#version 330 core

layout(location = 0) in vec3 inPos;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	gl_Position = uProj * uView * vec4(inPos, 1);
}