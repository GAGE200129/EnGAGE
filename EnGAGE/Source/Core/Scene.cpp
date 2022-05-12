#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "ECS.hpp"
#include "GameEngine.hpp"
#include "Physics.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/Script.hpp"
#include "Components/DirectionalLight.hpp"

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
static void writeComponent(std::ofstream& out, const String& entity, Core::ComponentType componentType, Core::ComponentHeader* header);

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
		for (unsigned int j = 0; j < (unsigned int)ComponentType::COUNT; j++)
		{
			auto& signatures = ECS::getEntitySignatures();
			ComponentHeader* pHeader = (ComponentHeader*)ECS::getComponent(signatures[i].id, (ComponentType)j);
			if (pHeader)
			{
				writeComponent(fileOut, entityName, (ComponentType)j, pHeader);
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

		LuaHostFunctions::pushAllGlobals(L);
		LuaHostFunctions::registerFunctions(L);

		checkLua(L, luaL_dofile(L, gScenePath.c_str()));

		lua_close(L);

		gSceneSwitch = false;
	}
}

String& Core::Scene::getLoadedSceneName()
{
	return gScenePath;
}

void writeComponent(std::ofstream& out, const String& entity, Core::ComponentType componentType, Core::ComponentHeader* header)
{
	using namespace Core;
	header->OnSeralize(header, out, entity);

}

