#include "pch.hpp"
#include "Map.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "Core/LuaHostFunctions.hpp"
#include "Core/Physics/Physics.hpp"
#include "Core/Resource.hpp"

namespace Core::Map
{
	static MapData gData;

	void init()
	{
		EN_INFO("Initializing map");
		glGenVertexArrays(1, &gData.wallMesh.vao);
		glGenBuffers(1, &gData.wallMesh.vbo);
		//glGenBuffers(1, &gData.wallMesh.ebo);

		gData.wallMesh.vertexCount = 0;
	}
	void serialize(std::ofstream& out)
	{
		if (auto* sheet = gData.wallMesh.textureSheet)
		{
			out << "_setTextureSheet(\"" << sheet->name << "\", " << sheet->div.x << ", " << sheet->div.y << ")\n";
		}
		for (const auto& face : Map::getData().faces)
		{
			const Vec3& p1 = face.p1;
			const Vec3& p2 = face.p2;
			const Vec3& p3 = face.p3;
			const Vec3& p4 = face.p4;

			out << "_addFace(" <<
				p1.x << ", " << p1.y << ", " << p1.z << ", " <<
				p2.x << ", " << p2.y << ", " << p2.z << ", " <<
				p3.x << ", " << p3.y << ", " << p3.z << ", " <<
				p4.x << ", " << p4.y << ", " << p4.z << ", " <<
				face.texSlot.x << ", " << face.texSlot.y << ")\n";
		}
	}
	void shutdown()
	{
		EN_INFO("Shutting down map");
		glDeleteVertexArrays(1, &gData.wallMesh.vao);
		glDeleteBuffers(1, &gData.wallMesh.vbo);
		//glDeleteBuffers(1, &gData.wallMesh.ebo);
	}


	void clear()
	{
		gData.wallMesh.textureSheet = nullptr;
		gData.faces.clear();
		updateVertexData();
	}

	int luaAddFace(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 14);
		CHECK_ARG(L, 1, LUA_TNUMBER);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);
		CHECK_ARG(L, 5, LUA_TNUMBER);
		CHECK_ARG(L, 6, LUA_TNUMBER);
		CHECK_ARG(L, 7, LUA_TNUMBER);
		CHECK_ARG(L, 8, LUA_TNUMBER);
		CHECK_ARG(L, 9, LUA_TNUMBER);
		CHECK_ARG(L, 10, LUA_TNUMBER);
		CHECK_ARG(L, 11, LUA_TNUMBER);
		CHECK_ARG(L, 12, LUA_TNUMBER);
		CHECK_ARG(L, 13, LUA_TNUMBER);
		CHECK_ARG(L, 14, LUA_TNUMBER);

		Vec3 p1 = { (F32)lua_tonumber(L, 1), (F32)lua_tonumber(L, 2), (F32)lua_tonumber(L, 3) };
		Vec3 p2 = { (F32)lua_tonumber(L, 4), (F32)lua_tonumber(L, 5), (F32)lua_tonumber(L, 6) };
		Vec3 p3 = { (F32)lua_tonumber(L, 7), (F32)lua_tonumber(L, 8), (F32)lua_tonumber(L, 9) };
		Vec3 p4 = { (F32)lua_tonumber(L,10), (F32)lua_tonumber(L,11), (F32)lua_tonumber(L,12) };
		Vec2I textureSlot = { lua_tointeger(L, 13), lua_tointeger(L, 14) };

		addFace(p1, p2, p3, p4, textureSlot);
		return 0;
	}

	int luaSetTextureSheet(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 3);
		CHECK_ARG(L, 1, LUA_TSTRING);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);

		const char* path = lua_tostring(L, 1);
		EN_ASSERT(path != nullptr, "Texture sheet path is nullptr !");

		String sheetPath = path;
		TextureSheet* sheet = Resource::getTextureSheet(sheetPath);
		if (sheet)
		{
			sheet->div = { lua_tointeger(L, 2), lua_tointeger(L, 3) };
			gData.wallMesh.textureSheet = sheet;
		}

		return 0;
	}


	MapData& getData()
	{
		return gData;
	}

	void addFace(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::ivec2& textureSlot)
	{
		Face face;
		face.p1 = p1;
		face.p2 = p2;
		face.p3 = p3;
		face.p4 = p4;
		face.texSlot = textureSlot;

		face.order = Face::TriangleOrder::TOP;
		gData.faces.push_back(face);
		updateVertexData();
	}
	void removeFace(size_t index)
	{
		EN_ASSERT(index < gData.faces.size(), "Face index out of bound !");
		gData.faces.erase(gData.faces.begin() + index);
		updateVertexData();
	}
	void updateVertexData()
	{
		struct Vertex
		{
			Vec3 position;
			Vec2 uv;
			Vec3 normal;
		};

		//Build vertices

		DynArr<Triangle> triangles; //For the physics engine
		DynArr<Vertex> vertices;//To be uploaded to GPU
		//DynArr<UInt32> indices;
		//UInt32 index = 0;
		for (auto& face : gData.faces)
		{
			//Update face triangle
			face.tri[0] = { face.p1, face.p2, face.p3 };
			face.tri[1] = { face.p1, face.p3, face.p4 };
			

			glm::vec3 normal1 = glm::normalize(glm::cross(face.p2 - face.p1, face.p3 - face.p1));
			glm::vec3 normal2 = glm::normalize(glm::cross(face.p3 - face.p1, face.p4 - face.p1));
			glm::vec3 normal = glm::mix(normal1, normal2, 0.5f);


			Vec2I div = { 1, 1 };
			if (auto* sheet = gData.wallMesh.textureSheet)
				div = sheet->div;
			float deltaX = 1.0f / div.x;
			float deltaY = 1.0f / div.y;

			face.t1 = { face.texSlot.x * deltaX, face.texSlot.y * deltaY }; //Bottom left
			face.t2 = { face.texSlot.x * deltaX + deltaX, face.texSlot.y * deltaY }; //Top left
			face.t3 = { face.texSlot.x * deltaX + deltaX, face.texSlot.y * deltaY + deltaY }; //Top Right
			face.t4 = { face.texSlot.x * deltaX, face.texSlot.y * deltaY + deltaY }; //Bottom right

			//Need extra face
			constexpr float FACE_THICNESS = 0.03f;
			Triangle tri1, tri2, tri3, tri4;
			tri1 = { face.tri[0].p1 + normal * FACE_THICNESS, face.tri[0].p2 + normal * FACE_THICNESS, face.tri[0].p3 + normal * FACE_THICNESS };
			tri2 = { face.tri[1].p1 + normal * FACE_THICNESS, face.tri[1].p2 + normal * FACE_THICNESS, face.tri[1].p3 + normal * FACE_THICNESS };
			tri3 = { face.tri[0].p3 - normal * FACE_THICNESS, face.tri[0].p2 - normal * FACE_THICNESS, face.tri[0].p1 - normal * FACE_THICNESS };
			tri4 = { face.tri[1].p3 - normal * FACE_THICNESS, face.tri[1].p2 - normal * FACE_THICNESS, face.tri[1].p1 - normal * FACE_THICNESS };

			vertices.push_back({ tri1.p1,  face.t1, normal });
			vertices.push_back({ tri1.p2,  face.t2, normal });
			vertices.push_back({ tri1.p3,  face.t3, normal });

			vertices.push_back({ tri2.p1,  face.t1, normal });
			vertices.push_back({ tri2.p2,  face.t3, normal });
			vertices.push_back({ tri2.p3,  face.t4, normal });

			vertices.push_back({ tri3.p1,  face.t3, -normal });
			vertices.push_back({ tri3.p2,  face.t2, -normal });
			vertices.push_back({ tri3.p3,  face.t1, -normal });

			vertices.push_back({ tri4.p1,  face.t4, -normal });
			vertices.push_back({ tri4.p2,  face.t3, -normal });
			vertices.push_back({ tri4.p3,  face.t1, -normal });

			triangles.push_back({ tri1 });
			triangles.push_back({ tri2 });
			triangles.push_back({ tri3 });
			triangles.push_back({ tri4 });
		}

		//Update gpu buffer
		glBindVertexArray(gData.wallMesh.vao);
		glBindBuffer(GL_ARRAY_BUFFER, gData.wallMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)20);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		gData.wallMesh.vertexCount = gData.faces.size() * 12;

		//Update physics engine map
		Physics::updateMap(triangles);
	}
}