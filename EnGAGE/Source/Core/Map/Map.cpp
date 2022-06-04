#include "pch.hpp"
#include "Map.hpp"

#include <glad/glad.h>

#include "Core/Physics.hpp"
#include "Core/Triangle.hpp"

namespace Core::Map
{
	static DynArr<Wall> gWalls;
	static WallMesh gWallMesh;

	void updateVertexData();

	void init()
	{
		EN_INFO("Initializing map");
		glGenVertexArrays(1, &gWallMesh.vao);
		glGenBuffers(1, &gWallMesh.vbo);
		glGenBuffers(1, &gWallMesh.ebo);

		gWallMesh.vertexCount = 0;
	}
	void serialize(std::ofstream& out)
	{
		if (auto* sheet = gWallMesh.textureSheet)
		{
			out << "_setTextureSheet(\"" << sheet->name << "\", " << sheet->div.x << ", " << sheet->div.y << ")\n";
		}
		for (const auto& wall : gWalls)
		{
			const Vec3& p1 = wall.p1;
			const Vec3& p2 = wall.p2;
			const Vec3& p3 = wall.p3;
			const Vec3& p4 = wall.p4;
			const Vec3& p5 = wall.p5;
			const Vec3& p6 = wall.p6;
			const Vec3& p7 = wall.p7;
			const Vec3& p8 = wall.p8;
			out << "_addWall(" <<
				p1.x << ", " << p1.y << ", " << p1.z << ", " <<
				p2.x << ", " << p2.y << ", " << p2.z << ", " <<
				p3.x << ", " << p3.y << ", " << p3.z << ", " <<
				p4.x << ", " << p4.y << ", " << p4.z << ", " <<
				p5.x << ", " << p5.y << ", " << p5.z << ", " <<
				p6.x << ", " << p6.y << ", " << p6.z << ", " <<
				p7.x << ", " << p7.y << ", " << p7.z << ", " <<
				p8.x << ", " << p8.y << ", " << p8.z << ", " <<
				wall.front.x << ", " << wall.front.y << ", " <<
				wall.back.x << ", " << wall.back.y << ", " <<
				wall.left.x << ", " << wall.left.y << ", " <<
				wall.right.x << ", " << wall.right.y << ", " <<
				wall.top.x << ", " << wall.top.y << ", " <<
				wall.bottom.x << ", " << wall.bottom.y << ")\n";
		}
	}
	void shutdown()
	{
		EN_INFO("Shutting down map");
		glDeleteVertexArrays(1, &gWallMesh.vao);
		glDeleteBuffers(1, &gWallMesh.vbo);
		glDeleteBuffers(1, &gWallMesh.ebo);
	}

	void addWall(Wall wall)
	{
		gWalls.push_back(wall);
		updateVertexData();
	}
	void removeWall(UInt64 index)
	{
		gWalls.erase(gWalls.begin() + index);
		updateVertexData();
	}
	void clear()
	{
		gWalls.clear();
		gWallMesh.textureSheet = nullptr;
	}

	WallMesh& getWallMesh()
	{
		return gWallMesh;
	}

	DynArr<Wall>& getWalls()
	{
		return gWalls;
	}

	void updateVertexData()
	{
		struct Vertex
		{
			Vec3 position;
			Vec2 uv;
			Vec3 normal;
		};
		auto addWallFace = [](Face face, const Vec2I& texSlot, DynArr<Face>& outFace)
		{
			Vec3 normal1 = glm::normalize(glm::cross(face.p2 - face.p1, face.p4 - face.p1));
			Vec3 normal2 = glm::normalize(glm::cross(face.p4 - face.p3, face.p2 - face.p3));
			face.normal = glm::mix(normal1, normal2, 0.5f);

			auto* sheet = gWallMesh.textureSheet;

			Vec2I div = { 1, 1 };
			if (sheet)
				div = sheet->div;

			F32 deltaX = 1.0f / div.x;
			F32 deltaY = 1.0f / div.y;

			face.t1 = { texSlot.x * deltaX, texSlot.y * deltaY}; //Bottom left
			face.t2 = { texSlot.x * deltaX + deltaX, texSlot.y * deltaY}; //Top left
			face.t3 = { texSlot.x * deltaX + deltaX, texSlot.y * deltaY + deltaY }; //Top Right
			face.t4 = { texSlot.x * deltaX, texSlot.y * deltaY + deltaY }; //Bottom right

			outFace.push_back(face);
		};

		//Build faces
		DynArr<Face> faces;
		faces.reserve(gWalls.size() * 6);
		for (const auto& wall : gWalls)
		{
			addWallFace(Face{ wall.p4, wall.p3, wall.p2, wall.p1 }, wall.bottom, faces); // Bottom
			addWallFace(Face{ wall.p5, wall.p6, wall.p7, wall.p8 }, wall.top, faces); // Top
			addWallFace(Face{ wall.p2, wall.p3, wall.p7, wall.p6 }, wall.back, faces); // backward
			addWallFace(Face{ wall.p4, wall.p1, wall.p5, wall.p8 }, wall.front, faces); // forward
			addWallFace(Face{ wall.p5, wall.p1, wall.p2, wall.p6 }, wall.left, faces); // Left
			addWallFace(Face{ wall.p3, wall.p4, wall.p8, wall.p7 }, wall.right, faces); // Right
		}


		//Build vertices

		DynArr<Vertex> vertices;
		DynArr<Triangle> triangles;
		DynArr<UInt32> indices;
		UInt32 index = 0;
		for (const auto& face : faces)
		{
			vertices.push_back({ face.p1,  face.t1, face.normal });
			vertices.push_back({ face.p2,  face.t2, face.normal });
			vertices.push_back({ face.p3,  face.t3, face.normal });
			vertices.push_back({ face.p4,  face.t4, face.normal });
			indices.push_back(index + 0);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
			indices.push_back(index + 0);
			indices.push_back(index + 2);
			indices.push_back(index + 3);
			index += 4;


			triangles.push_back({ face.p1, face.p2, face.p3 });
			triangles.push_back({ face.p1, face.p3, face.p4 });
		}

		//Update gpu buffer
		glBindVertexArray(gWallMesh.vao);
		glBindBuffer(GL_ARRAY_BUFFER, gWallMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)20);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gWallMesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(UInt32), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		gWallMesh.vertexCount = gWalls.size() * 36;

		//Update physics engine map
		Physics::updateMap(triangles);
	}
}