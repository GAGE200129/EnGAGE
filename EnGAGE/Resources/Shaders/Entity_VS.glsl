#version 330 core

#define MAX_BONES 100

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in ivec4 inBoneIDs;
layout(location = 4) in vec4 inBoneWeights;

out vec3 FSPosition;
out vec3 FSNormal;
out vec2 FSTexCoord;

uniform mat4 uProjView;
uniform mat4 uModel;
uniform int uSkinned;
uniform mat4 uBones[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0f);

	if(uSkinned == 1)
	{
		for(int i = 0; i < 4; i++)
		{
			totalPosition += inBoneWeights[i] * uBones[inBoneIDs[i]] * vec4(inPos, 1);		
		}
	}
	else
	{
		totalPosition = vec4(inPos, 1);
	}
	vec4 position = uModel * totalPosition; 

	gl_Position = uProjView * position;
	FSPosition = position.xyz;
	FSNormal = (mat3(uModel) * inNormal).xyz;
	FSTexCoord = inTexCoord;
}