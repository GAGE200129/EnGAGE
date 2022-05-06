#version 330 core

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(0, 0, 0, 1.0);
}