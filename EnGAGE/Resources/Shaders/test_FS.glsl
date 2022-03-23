#version 330 core

in vec2 FSTexCoord;

out vec4 out_Color;

uniform sampler2D uBaseColor;

void main()
{
	out_Color = vec4(1, 0, 0, 1);
}