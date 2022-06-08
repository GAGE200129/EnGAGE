#pragma once

#include "Core/Data/Model.hpp"
#include "TextureSheet.hpp"
#include "WallMesh.hpp"

namespace Core
{
	struct Face
	{
		Vec3 p1, p2, p3, p4;
		Vec2 t1, t2, t3, t4;
		Vec3 normal;
	};

	struct Wall
	{
		union
		{
			struct
			{

				Vec3 p1, p2, p3, p4;
				Vec3 p5, p6, p7, p8;

			};
			Vec3 p[8];
		};
		union
		{
			struct
			{

				Vec2I front, back, left, right, top, bottom;

			};
			Vec2I slotCoords[6] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0} };
		};
	};

	
	namespace Map
	{
		void init();
		void serialize(std::ofstream& out);
		void clear();
		void shutdown();
		void addWall(Wall wall);
		void removeWall(UInt64 index);
		DynArr<Wall>& getWalls();
		WallMesh& getWallMesh();
		void updateVertexData();
	}
}