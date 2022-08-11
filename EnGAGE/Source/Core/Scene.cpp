#include "pch.hpp"
#include "Scene.hpp"

#include "LuaHostFunctions.hpp"
#include "Core/ECS/ECS.hpp"
#include "GameEngine.hpp"
#include "Core/Physics/Physics.hpp"
#include "Math.hpp"
#include "Core/Data/Model.hpp"
#include "Core/Data/Vertex.hpp"
#include "Map/Map.hpp"
#include "ECS/RigidBody.hpp"
#include "ECS/Transform.hpp"
#include "ECS/ModelRenderer.hpp"
#include "ECS/Script.hpp"
#include "ECS/DirectionalLight.hpp"

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
	static SceneData gData;

	void loadScene(const String& filePath)
	{
		gData.sceneSwitch = true;
		gData.scenePath = filePath;
	}

	void saveScene(const String& filePath)
	{
		gData.scenePath = filePath;
		std::ofstream fileOut(filePath);

		
		for (UInt64 i = 0; i < ECS::getEntityCount(); i++)
		{
			const ECS::EntitySignature& signature = ECS::getEntitySignatures()[i];
			String entityName = "entity";
			fileOut << "local " << entityName << " = _createEntity()\n";
			//Unload all components
			for (unsigned int j = 0; j < (unsigned int)ComponentType::COUNT; j++)
			{
				auto& signatures = ECS::getEntitySignatures();
				ComponentHeader* pHeader = (ComponentHeader*)ECS::getComponent(signature.id, (ComponentType)j);
				if (pHeader)
				{
					pHeader->OnSeralize(pHeader, fileOut, entityName);
				}
			}

		}
		//Serialize the map
		Map::serialize(fileOut);

		fileOut.close();
	}

	void checkForSceneSwitch()
	{
		if (gData.sceneSwitch)
		{
			EN_INFO("Switching scene: {}", gData.scenePath);
			GameEngine::clearResources();

			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			LuaHostFunctions::pushAllGlobals(L);
			LuaHostFunctions::registerFunctions(L);

			checkLua(L, luaL_dofile(L, gData.scenePath.c_str()));

			lua_close(L);

			gData.sceneSwitch = false;
		}
	}
	
	const SceneData& getData()
	{
		return gData;
	}
}
