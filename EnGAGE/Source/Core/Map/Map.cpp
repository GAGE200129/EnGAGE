#include "pch.hpp"
#include "Map.hpp"

#include <glad/glad.h>

namespace Core::Map
{
	static DynArr<Wall> gWalls;
	static UInt32 gWallVAO, gWallVBO, gWallEBO;

	void updateVertexData();
	
	void init()
	{
		EN_INFO("Initializing map");
		glGenVertexArrays(1, &gWallVAO);
		glGenBuffers(1, &gWallVBO);
		glGenBuffers(1, &gWallEBO);
	}

	void shutdown()
	{
		EN_INFO("Shutting down map");
		glDeleteVertexArrays(1, &gWallVAO);
		glDeleteBuffers(1, &gWallVBO);
		glDeleteBuffers(1, &gWallEBO);
	}

	void addWall(Wall wall)
	{
		Vec3 normal1 = glm::normalize(glm::cross(wall.p2 - wall.p1, wall.p4 - wall.p1));
		Vec3 normal2 = glm::normalize(glm::cross(wall.p4 - wall.p3, wall.p2 - wall.p3));
		wall.normal = glm::mix(normal1, normal2, 0.5f);
		wall.t1 = { 0, 0 };
		wall.t2 = { 0, 1 };
		wall.t3 = { 1, 1 };
		wall.t4 = { 1, 0 };
		gWalls.push_back(wall);
		updateVertexData();
	}

	void clear()
	{
		gWalls.clear();
	}

	UInt32 getWallMesh()
	{
		return gWallVAO;
	}

	DynArr<Wall>& getWalls()
	{
		return gWalls;
	}

	static void updateVertexData()
	{
		//Build vertices
		struct Vertex
		{
			Vec3 position;
			Vec2 uv;
			Vec3 normal;
		};
		DynArr<Vertex> vertices;
		DynArr<UInt32> indices;
		UInt32 index = 0;
		for (const auto& wall : gWalls)
		{
			vertices.push_back({ wall.p1, wall.t1, wall.normal });
			vertices.push_back({ wall.p2, wall.t2, wall.normal });
			vertices.push_back({ wall.p3, wall.t3, wall.normal });
			vertices.push_back({ wall.p4, wall.t4, wall.normal });
			indices.push_back(index + 0);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
			indices.push_back(index + 0);
			indices.push_back(index + 2);
			indices.push_back(index + 3);
			index += 4;

		}

		//Update gpu buffer
		glBindVertexArray(gWallVAO);
		glBindBuffer(GL_ARRAY_BUFFER, gWallVAO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 5));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gWallEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(UInt32), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
}