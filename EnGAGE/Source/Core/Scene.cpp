#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
static bool checkLua(lua_State* L, int r);
static void writeComponent(std::ofstream& out, const String& entity, Core::ECS::ComponentType componentType, Core::ECS::ComponentHeader* header);
static void writeCollisionShape(std::ofstream& out, const String& componentName, Core::ECS::RigidBodyComponent* component);

static bool gSceneSwitch = false;
static String gScenePath = "";

void Core::Scene::loadScene(const String& filePath)
{
	gSceneSwitch = true;
	gScenePath = filePath;
}

void Core::Scene::saveScene(const String& filePath)
{
	std::ofstream fileOut(filePath);

	for (unsigned int i = 0; i < ECS::getEntityCount(); i++)
	{
		String entityName = "entity" + std::to_string(i);
		fileOut << entityName  << " = _createEntity()\n";

		//Unload all components
		for (unsigned int j = 0; j < (unsigned int)ECS::ComponentType::COUNT; j++)
		{
			auto& signatures = ECS::getEntitySignatures();
			ECS::ComponentHeader* pHeader = (ECS::ComponentHeader*)ECS::getComponent(signatures[i].id, (ECS::ComponentType)j);
			if (pHeader)
			{
				writeComponent(fileOut, entityName, (ECS::ComponentType)j, pHeader);
			}
		}
		fileOut << "\n";
	}

	fileOut.close();
}

void Core::Scene::checkForSceneSwitch()
{
	if (gSceneSwitch)
	{
		EN_INFO("Switching scene: {}", gScenePath);
		GameEngine::clearResources();

		lua_State* L = luaL_newstate();
		luaL_openlibs(L);

		for (unsigned int i = 0; i < (unsigned int)ECS::ComponentType::COUNT; i++)
		{
			ECS::ComponentData data = ECS::getComponentData((ECS::ComponentType)i);
			lua_pushinteger(L, i);
			lua_setglobal(L, data.name);
		}
		LuaHostFunctions::registerAllSceneFunctions(L);

		checkLua(L, luaL_dofile(L, gScenePath.c_str()));

		lua_close(L);

		gSceneSwitch = false;
	}
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

void writeComponent(std::ofstream& out, const String& entity, Core::ECS::ComponentType componentType, Core::ECS::ComponentHeader* header)
{
	auto data = Core::ECS::getComponentData(componentType);
	switch (componentType)
	{
	case Core::ECS::ComponentType::NAME:
		break;
	case Core::ECS::ComponentType::TRANSFORM:	
	{
		String componentName = entity + "_transform";
		out << componentName << " = _addComponent(" << entity << ", " << data.name << ")\n";
		Core::ECS::TransformComponent* component = (Core::ECS::TransformComponent*)header;
		out << "_setPosition(" << componentName << ", " << component->x << ", " << component->y << ", " << component->z << ")\n";
		out << "_setRotation(" << componentName << ", " << component->rw << ", " << component->rx << ", " << component->ry << ", " << component->rz << ")\n";
		out << "_setScale(" << componentName << ", " << component->sx << ", " << component->sy << ", " << component->sz << ")\n";
		break;
	}
	case Core::ECS::ComponentType::MODEL_RENDERER:
	{
		Core::ECS::ModelRendererComponent* component = (Core::ECS::ModelRendererComponent*)header;
		out << "_setModel(" << "_addComponent(" << entity << ", " << data.name << ")" << ", \"" << component->modelPath << "\")\n";
		break;
	}
	case Core::ECS::ComponentType::SCRIPT:
	{
		Core::ECS::ScriptComponent* component = (Core::ECS::ScriptComponent*)header;
		out << "_setScript(" << "_addComponent(" << entity << ", " << data.name << ")" << ", \"" << component->scriptPath << "\")\n";
		break;
	}
	case Core::ECS::ComponentType::RIGID_BODY:
	{
		String componentName = entity + "_rigidBody";
		out << componentName << " = _addComponent(" << entity << ", " << data.name << ")\n";
		Core::ECS::RigidBodyComponent* component = (Core::ECS::RigidBodyComponent*)header;
		out << "_setRigidBody(" << componentName << ", "
			<< component->velocity.x << ", " << component->velocity.y << ", " << component->velocity.z << ", "
			<< component->force.x << ", " << component->force.y << ", " << component->force.z << ", "
			<< component->mass
			<< ")\n";

		writeCollisionShape(out, componentName, component);
		break;
	}
	}
}

void writeCollisionShape(std::ofstream& out, const String& componentName, Core::ECS::RigidBodyComponent* component)
{
	switch (component->colliderType)
	{
	case Core::ECS::ColliderType::NONE:

	case Core::ECS::ColliderType::SPHERE: // Float(radius)
	{
		Core::ECS::SphereCollider* collider = (Core::ECS::SphereCollider*)component->colliderData;
		out << "_setCollisionShapeSphere(" << componentName << ", " << collider->radius << ")\n";
		break;
	}
	case Core::ECS::ColliderType::PLANE: // Vec3(Normal), Float(distance)
	{
		Core::ECS::PlaneCollider* collider = (Core::ECS::PlaneCollider*)component->colliderData;
		out << "_setCollisionShapePlane(" << componentName << ", " << collider->x << ", " << collider->y << ", " << collider->z << ", " << collider->distance << ")\n";
		break;
	}
	}
}
