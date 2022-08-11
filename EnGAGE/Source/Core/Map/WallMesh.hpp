#pragma once

namespace Core
{
	struct TextureSheet;
	struct WallMesh
	{
		UInt32 vao = 0, vbo = 0;
		UInt64 vertexCount = 0;

		TextureSheet* textureSheet = nullptr;
	};
}