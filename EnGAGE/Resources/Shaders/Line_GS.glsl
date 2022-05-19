#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 uMVP;
uniform vec3 uBegin;
uniform vec3 uEnd;

void main()
{
    gl_Position =  uMVP * vec4(uBegin, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(uEnd, 1.0);
    EmitVertex();
    
    EndPrimitive();
}