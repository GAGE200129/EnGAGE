#version 330 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 FSTexCoord;

void main()
{
	FSTexCoord = inTexCoord;
	gl_Position = vec4(inPosition, 0, 1);
}