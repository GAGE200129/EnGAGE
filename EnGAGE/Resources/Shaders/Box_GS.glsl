#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 16) out;

uniform mat4 uMVP;
uniform vec3 uMin;
uniform vec3 uMax;

void main()
{

    vec3 p1 = uMin;
    vec3 p2 = vec3(uMin.x, uMin.y, uMax.z);
    vec3 p3 = vec3(uMax.x, uMin.y, uMax.z);
    vec3 p4 = vec3(uMax.x, uMin.y, uMin.z);

    vec3 p5 = vec3(uMin.x, uMax.y, uMin.z);
    vec3 p6 = vec3(uMin.x, uMax.y, uMax.z);
    vec3 p7 = vec3(uMax.x, uMax.y, uMax.z);
    vec3 p8 = vec3(uMax.x, uMax.y, uMin.z);

    gl_Position =  uMVP * vec4(p1, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p2, 1.0);
    EmitVertex();

    gl_Position =  uMVP * vec4(p3, 1.0);
    EmitVertex();

    gl_Position =  uMVP * vec4(p4, 1.0);
    EmitVertex();

    gl_Position =  uMVP * vec4(p1, 1.0);
    EmitVertex();

    gl_Position =  uMVP * vec4(p5, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p6, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p7, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p8, 1.0); 
    EmitVertex();

     gl_Position =  uMVP * vec4(p5, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p6, 1.0); 
    EmitVertex();

     gl_Position =  uMVP * vec4(p2, 1.0);
    EmitVertex();

    gl_Position =  uMVP * vec4(p3, 1.0);
    EmitVertex();

     gl_Position =  uMVP * vec4(p7, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p8, 1.0); 
    EmitVertex();

    gl_Position =  uMVP * vec4(p4, 1.0);
    EmitVertex();

    EndPrimitive();
}