#pragma once

#include "Core/Data/Model.hpp"
#include "TextureSheet.hpp"
#include "WallMesh.hpp"
#include "Core/Data/Triangle.hpp"

namespace Core
{
	struct Face
	{
		enum class TriangleOrder
		{
			TOP,
			BOTTOM
		};
		union
		{
			struct
			{
				glm::vec3 p1, p2, p3, p4;
			};
			glm::vec3 p[4];
		};
		glm::ivec2 texSlot;
		glm::vec2 t1, t2, t3, t4;
		Triangle tri[2];
		TriangleOrder order;
	};


	struct MapData
	{
		WallMesh wallMesh;
		std::vector<Face> faces;
	};
	namespace Map
	{
		void init();
		void serialize(std::ofstream& out);
		void clear();
		void addFace(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::ivec2& textureSlot);
		void removeFace(size_t index);
		void shutdown();
		MapData& getData();
		void updateVertexData();

		int luaAddFace(lua_State* L);
		int luaSetTextureSheet(lua_State* L);
	}
}