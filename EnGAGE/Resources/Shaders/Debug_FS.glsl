#version 330 core

uniform vec3 uColor;

out vec4 out_Color;

void main()
{
	out_Color = vec4(uColor, 1);
}
