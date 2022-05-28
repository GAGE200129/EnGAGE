#pragma once

namespace Core
{
	struct Wall
	{
		Vec3 p1, p2, p3, p4;
		Vec2 t1, t2, t3, t4;
		Vec3 normal;
	};

	namespace Map
	{
		void init();
		void clear();
		void shutdown();
		void addWall(Wall wall);
		DynArr<Wall>& getWalls();
		UInt32 getWallMesh();
	}
}