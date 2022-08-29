#pragma once

namespace Core
{
	struct Vertex
	{
		float x, y, z;
		float nX, nY, nZ;
		float tU, tV;
		unsigned int boneIDs[4];
		float boneWeights[4];
	};
}