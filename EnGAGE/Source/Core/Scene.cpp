#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"
#include "Physics.hpp"

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

		for (unsigned int i = 0; i < (unsigned int)Physics::ColliderType::COUNT; i++)
		{
			const Physics::ColliderData& data = Physics::getColliderData((Physics::ColliderType)i);
			lua_pushinteger(L, i);
			lua_setglobal(L, data.name);
		}
		LuaHostFunctions::registerAllSceneFunctions(L);

		checkLua(L, luaL_dofile(L, gScenePath.c_str()));

		lua_close(L);

		gSceneSwitch = false;
	}
}

String& Core::Scene::getLoadedSceneName()
{
	return gScenePath;
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
	using namespace Core::ECS;
	auto data = getComponentData(componentType);
	switch (componentType)
	{
	case ComponentType::NAME:
		break;
	case ComponentType::TRANSFORM:	
	{
		String componentName = entity + "_transform";
		out << componentName << " = _addComponent(" << entity << ", " << data.name << ")\n";
		TransformComponent* component = (TransformComponent*)header;
		out << "_setPosition(" << componentName << ", " << component->x << ", " << component->y << ", " << component->z << ")\n";
		out << "_setRotation(" << componentName << ", " << component->rw << ", " << component->rx << ", " << component->ry << ", " << component->rz << ")\n";
		out << "_setScale(" << componentName << ", " << component->sx << ", " << component->sy << ", " << component->sz << ")\n";
		break;
	}
	case ComponentType::MODEL_RENDERER:
	{
		ModelRendererComponent* component = (ModelRendererComponent*)header;
		out << "_setModel(" << "_addComponent(" << entity << ", " << data.name << ")" << ", \"" << component->modelPath << "\")\n";
		break;
	}
	case ComponentType::SCRIPT:
	{
		ScriptComponent* component = (ScriptComponent*)header;
		out << "_setScript(" << "_addComponent(" << entity << ", " << data.name << ")" << ", \"" << component->scriptPath << "\")\n";
		break;
	}
	case ComponentType::RIGID_BODY:
	{
		String componentName = entity + "_rigidBody";
		out << componentName << " = _addComponent(" << entity << ", " << data.name << ")\n";
		RigidBodyComponent* component = (RigidBodyComponent*)header;
		out << "_setRigidBody(" << componentName << ", "
			<< component->velocity.x << ", " << component->velocity.y << ", " << component->velocity.z << ", "
			<< component->force.x << ", " << component->force.y << ", " << component->force.z << ", "
			<< component->mass
			<< ")\n";

		writeCollisionShape(out, componentName, component);
		break;
	}
	case ComponentType::DIRECTIONAL_LIGHT:
	{
		DirectionalLightComponent* component = (DirectionalLightComponent*)header;

		out << "_setDirectionalLight(_addComponent(" << entity << ", " << data.name << "), " <<
			component->direction.x << ", " << component->direction.y << ", " << component->direction.z << ", " <<
			component->color.x << ", " << component->color.y << ", " << component->color.z << ", " <<
			component->intensity << ")\n";
		break;
	}
	}
}

void writeCollisionShape(std::ofstream& out, const String& componentName, Core::ECS::RigidBodyComponent* component)
{
	using namespace Core::Physics;
	out << "_setCollisionShape(";

	auto type = (ColliderType)component->colliderType;
	const ColliderData& data = getColliderData(type);
	switch (type)
	{
	case ColliderType::SPHERE: // Float(radius)
	{
		SphereCollider* collider = (SphereCollider*)component->colliderData;
		out  << componentName << ", " << data.name << ", " << collider->radius << ")\n";
		break;
	}
	case ColliderType::PLANE: // Vec3(Normal), Float(distance)
	{
		PlaneCollider* collider = (PlaneCollider*)component->colliderData;
		out  << componentName << ", " << data.name << ", " << collider->x << ", " << collider->y << ", " << collider->z << ", " << collider->distance << ")\n";
		break;
	}
	}
}
