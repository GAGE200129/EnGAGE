#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Vertex.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/Script.hpp"
#include "Components/DirectionalLight.hpp"

#include <glad/glad.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
static bool checkLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		EN_ERROR("Lua error: {}", lua_tostring(L, -1));
		return false;
	}

	return true;
}

namespace Core::Scene
{
	
	

	static constexpr UInt32 MAX_WALLS = 1024;
	static bool gSceneSwitch = false;
	static String gScenePath = "";
	static DynArr<Ref<Wall>> gWalls;
	static UInt32 gMapVAO, gMapVBO, gMapEBO, gMapVertexCount;

	static void updateMap()
	{
		DynArr<Vertex> vertices;
		DynArr<Vertex> builtVertices;
		DynArr<UInt32> indices;
		unsigned int index = 0;
		for (const auto& wall : gWalls)
		{
			for (UInt32 i = 0; i < 4; i++)
			{
				Vertex vertex;
				vertex.x = wall->p[i].x;
				vertex.y = wall->p[i].y;
				vertex.z = wall->p[i].z;
				vertex.nX = wall->normal.x;
				vertex.nY = wall->normal.y;
				vertex.nZ = wall->normal.z;
				vertex.tU = 0;
				vertex.tV = 0;
				vertices.push_back(vertex);

			}

			indices.push_back(index + 0);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
			indices.push_back(index + 0);
			indices.push_back(index + 2);
			indices.push_back(index + 3);
			index += 4;
		}

		glBindVertexArray(gMapVAO);
		glBindBuffer(GL_ARRAY_BUFFER, gMapVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 6));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMapEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UInt32) * indices.size(), indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);

		gMapVertexCount = gWalls.size() * 6;

		for (const auto& index : indices)
		{
			builtVertices.push_back(vertices[index]);
		}
		Physics::updateMap(builtVertices);
	}

	void init()
	{
		glGenVertexArrays(1, &gMapVAO);
		glGenBuffers(1, &gMapVBO);
		glGenBuffers(1, &gMapEBO);
		gWalls.reserve(MAX_WALLS);
		updateMap();
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &gMapVAO);
		glDeleteBuffers(1, &gMapVBO);
		glDeleteBuffers(1, &gMapEBO);
	}
	void addWall(Vec3  p1, Vec3  p2, Vec3  p3, Vec3  p4)
	{
		if (gWalls.size() >= MAX_WALLS)
		{
			EN_WARN("Cannot add more wall, max: {}", MAX_WALLS);
			return;
		}
		Vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
		gWalls.push_back(createRef<Wall>(Wall{ p1, p2, p3, p4, normal }));
		updateMap();
	}

	void render()
	{
		std::function<void(const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)> processNode;
		processNode = [&processNode](const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)
		{
			accumulatedTransform = glm::translate(accumulatedTransform, node.position);
			accumulatedTransform *= glm::toMat4(node.rotation);
			accumulatedTransform = glm::scale(accumulatedTransform, node.scale);

			if (node.meshIndex != -1)
			{
				const Core::Mesh& mesh = model->meshes[node.meshIndex];
				for (const auto& primitive : mesh.primitives)
				{
					Renderer::submitMesh(primitive.vao, primitive.vertexCoumt, primitive.eboDataType, accumulatedTransform, true, mesh.boundingSphereRadius);
				}
			}
			for (const auto& child : node.children)
			{
				processNode(model, model->nodes[child], accumulatedTransform);
			}
		};


		System& system = ECS::getSystem(SystemType::RENDERER);
		for (auto e : system.entities)
		{
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);
			ModelRenderer::Component* pModelComp = (ModelRenderer::Component*)ECS::getComponent(e, ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				const Model* pModel = pModelComp->pModel;
				glm::mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(pModel, pModel->nodes[pModel->rootNodeIndex], modelMat);
			}
		}
		Renderer::submitMesh(gMapVAO, gMapVertexCount, GL_UNSIGNED_INT);
	}

	void loadScene(const String& filePath)
	{
		gSceneSwitch = true;
		gScenePath = filePath;
	}

	void saveScene(const String& filePath)
	{
		std::ofstream fileOut(filePath);

		for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
		{
			String entityName = "entity";
			fileOut << entityName << " = _createEntity()\n";

			//Unload all components
			for (unsigned int j = 0; j < (unsigned int)ComponentType::COUNT; j++)
			{
				auto& signatures = ECS::getEntitySignatures();
				ComponentHeader* pHeader = (ComponentHeader*)ECS::getComponent(signatures[i].id, (ComponentType)j);
				if (pHeader)
				{
					pHeader->OnSeralize(pHeader, fileOut, entityName);
				}
			}
			fileOut << "\n";
		}

		fileOut.close();
	}

	void checkForSceneSwitch()
	{
		if (gSceneSwitch)
		{
			EN_INFO("Switching scene: {}", gScenePath);
			GameEngine::clearResources();

			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			LuaHostFunctions::pushAllGlobals(L);
			LuaHostFunctions::registerFunctions(L);

			checkLua(L, luaL_dofile(L, gScenePath.c_str()));

			lua_close(L);

			gSceneSwitch = false;
		}
	}

	DynArr<Ref<Wall>>& getWallArray()
	{
		return gWalls;
	}

	String& getLoadedSceneName()
	{
		return gScenePath;
	}
}
