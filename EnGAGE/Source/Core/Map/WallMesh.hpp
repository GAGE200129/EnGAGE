#pragma once



namespace Core
{
	struct TextureSheet;
	struct WallMesh
	{
		//static constexpr UInt8 MAX_SHEETS = 8;
		UInt32 vao, vbo, ebo;
		UInt64 vertexCount;

		TextureSheet* textureSheet = nullptr;
	};
}