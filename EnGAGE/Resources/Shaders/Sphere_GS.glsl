#version 330 core

#define PI 3.1415926535897932384626433832795

layout (points) in;
layout (line_strip, max_vertices = 72) out;

uniform mat4 uMVP;
uniform float uRadius;

const int latDiv = 6;
const int longDiv = 12;


mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec3 base = vec3( 0.0f,0.0f,uRadius );
    const float latDeltaAngle = PI / latDiv;
	const float longDeltaAngle = 2.0f * PI / longDiv;

    const int halfLatDiv = 3;

    for( int iLat = -halfLatDiv; iLat < halfLatDiv; iLat++ )
	{
        for( int iLong = 0; iLong < (longDiv + 1); iLong++ )
	    {
            vec3 rotated = (rotationMatrix(vec3(0, 1, 0), longDeltaAngle * iLong) * rotationMatrix(vec3(1, 0, 0), latDeltaAngle * iLat) * vec4(base, 0)).xyz;
		    gl_Position = uMVP * vec4(rotated, 1.0); 
            EmitVertex();
	    }
	}

    


    EndPrimitive();
}