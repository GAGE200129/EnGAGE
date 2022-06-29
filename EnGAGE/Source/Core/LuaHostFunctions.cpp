#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "Core/ECS/ECS.hpp"
#include "InputCodes.hpp"
#include "Renderer/Renderer.hpp"
#include "Resource.hpp"
#include "Scripting.hpp"
#include "Scene.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Core/Physics/Physics.hpp"
#include "Input.hpp"
#include "GameEngine.hpp"
#include "Core/ECS/RigidBody.hpp"
#include "Core/ECS/Transform.hpp"
#include "Core/ECS/ModelRenderer.hpp"
#include "Core/ECS/Script.hpp"
#include "Core/ECS/DirectionalLight.hpp"
#include "Core/ECS/PointLight.hpp"
#include "Map/Map.hpp"

#include <BulletDynamics/Dynamics/btRigidBody.h>



namespace Core
{
	

	int getInt(lua_State* L)
	{
		CHECK_ARG(L, 1, LUA_TSTRING);
		CHECK_ARG(L, 2, LUA_TNUMBER);

		const char* messageData = lua_tostring(L, 1);
		unsigned int count = (unsigned int)lua_tointeger(L, 2);


		for (unsigned int i = 0; i < count; i++)
		{
			int interpretedInt;
			memcpy(&interpretedInt, messageData + i * sizeof(unsigned int), sizeof(unsigned int));
			lua_pushinteger(L, interpretedInt);
		}

		return count;
	}

	int getFloat(lua_State* L)
	{
		CHECK_ARG(L, 1, LUA_TSTRING);
		CHECK_ARG(L, 2, LUA_TNUMBER);

		const char* messageData = lua_tostring(L, 1);
		unsigned int count = (unsigned int)lua_tointeger(L, 2);


		for (unsigned int i = 0; i < count; i++)
		{
			float interpretedFloat;
			memcpy(&interpretedFloat, messageData + i * sizeof(float), sizeof(float));
			lua_pushnumber(L, interpretedFloat);
		}

		return count;
	}

	int sendMessage(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TNUMBER);
		CHECK_ARG(L, 2, LUA_TTABLE);

		lua_Debug ar;
		lua_getstack(L, 1, &ar);
		lua_getinfo(L, "nSl", &ar);
		int line = ar.currentline;
		const char* fileName = ar.source;

		EN_ASSERT(Messenger::processLuaMessage(L) != false, "In file: {}, line: {}, Calling: {}, Message not suported!", fileName, line, __func__);
		return 0;
	}

	int loadScene(lua_State* L)
	{
		EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
		const char* scenePath = lua_tostring(L, 1);
		EN_ASSERT(scenePath != nullptr, "scenePath is null");

		Core::Scene::loadScene(String(scenePath));

		return 0;
	}
	int saveScene(lua_State* L)
	{
		EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
		const char* scenePath = lua_tostring(L, 1);
		EN_ASSERT(scenePath != nullptr, "scenePath is null");

		Core::Scene::saveScene(String(scenePath));

		return 0;
	}
	int addWall(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 36);
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
		CHECK_ARG(L, 15, LUA_TNUMBER);
		CHECK_ARG(L, 16, LUA_TNUMBER);
		CHECK_ARG(L, 17, LUA_TNUMBER);
		CHECK_ARG(L, 18, LUA_TNUMBER);
		CHECK_ARG(L, 19, LUA_TNUMBER);
		CHECK_ARG(L, 20, LUA_TNUMBER);
		CHECK_ARG(L, 21, LUA_TNUMBER);
		CHECK_ARG(L, 22, LUA_TNUMBER);
		CHECK_ARG(L, 23, LUA_TNUMBER);
		CHECK_ARG(L, 24, LUA_TNUMBER);

		CHECK_ARG(L, 25, LUA_TNUMBER);
		CHECK_ARG(L, 26, LUA_TNUMBER);
		CHECK_ARG(L, 27, LUA_TNUMBER);
		CHECK_ARG(L, 28, LUA_TNUMBER);
		CHECK_ARG(L, 29, LUA_TNUMBER);
		CHECK_ARG(L, 30, LUA_TNUMBER);
		CHECK_ARG(L, 31, LUA_TNUMBER);
		CHECK_ARG(L, 32, LUA_TNUMBER);
		CHECK_ARG(L, 33, LUA_TNUMBER);
		CHECK_ARG(L, 34, LUA_TNUMBER);
		CHECK_ARG(L, 35, LUA_TNUMBER);
		CHECK_ARG(L, 36, LUA_TNUMBER);

		Vec3 p1 = { (F32)lua_tonumber(L, 1), (F32)lua_tonumber(L, 2), (F32)lua_tonumber(L, 3) };
		Vec3 p2 = { (F32)lua_tonumber(L, 4), (F32)lua_tonumber(L, 5), (F32)lua_tonumber(L, 6) };
		Vec3 p3 = { (F32)lua_tonumber(L, 7), (F32)lua_tonumber(L, 8), (F32)lua_tonumber(L, 9) };
		Vec3 p4 = { (F32)lua_tonumber(L,10), (F32)lua_tonumber(L,11), (F32)lua_tonumber(L,12) };
		Vec3 p5 = { (F32)lua_tonumber(L,13), (F32)lua_tonumber(L,14), (F32)lua_tonumber(L,15) };
		Vec3 p6 = { (F32)lua_tonumber(L,16), (F32)lua_tonumber(L,17), (F32)lua_tonumber(L,18) };
		Vec3 p7 = { (F32)lua_tonumber(L,19), (F32)lua_tonumber(L,20), (F32)lua_tonumber(L,21) };
		Vec3 p8 = { (F32)lua_tonumber(L,22), (F32)lua_tonumber(L,23), (F32)lua_tonumber(L,24) };
		Vec2I front = { lua_tointeger(L, 25), lua_tointeger(L, 26) };
		Vec2I back = { lua_tointeger(L, 27), lua_tointeger(L, 28) };
		Vec2I left = { lua_tointeger(L, 29), lua_tointeger(L, 30) };
		Vec2I right = { lua_tointeger(L, 31), lua_tointeger(L, 32) };
		Vec2I top = { lua_tointeger(L, 33) , lua_tointeger(L, 34) };
		Vec2I bottom = { lua_tointeger(L, 35), lua_tointeger(L, 36) };

		Map::addWall({ p1, p2, p3, p4, p5, p6, p7, p8, front, back, left, right, top, bottom });
		return 0;
	}

	int setTextureSheet(lua_State* L)
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
			Map::getWallMesh().textureSheet = sheet;
		}

		return 0;
	}

	namespace LuaHostFunctions
	{
		void checkNumArguments(lua_State* L, unsigned int arguments, const char* functionName)
		{
			lua_Debug ar;
			lua_getstack(L, 1, &ar);
			lua_getinfo(L, "nSl", &ar);
			int line = ar.currentline;
			const char* fileName = ar.source;

			unsigned int numArguments = lua_gettop(L);
			EN_ASSERT(numArguments == arguments, "In file: {}, calling lua's host function: {}, line: {}, Invalid numbers of argument: {}, expected: {}", fileName, functionName, line, numArguments, arguments);
		}

		void checkArgument(lua_State* L, int index, unsigned int expectedType, const char* functionName)
		{
			lua_Debug ar;
			lua_getstack(L, 1, &ar);
			lua_getinfo(L, "nSl", &ar);
			int line = ar.currentline;
			const char* fileName = ar.source;

			int type = lua_type(L, index);
			const char* argumentType = lua_typename(L, type);
			const char* expectedArgumentType = lua_typename(L, expectedType);
			EN_ASSERT(type == expectedType, "In file: {}, calling lua's host function: {}, line: {}, Invalid argument {}, expected: {}, got: {}", fileName, functionName, line, index, expectedArgumentType, argumentType);
		}

		void registerFunctions(lua_State* L)
		{
			lua_register(L, "_createEntity", Core::ECS::luaCreateEntity);
			lua_register(L, "_markForRemove", Core::ECS::luaMarkForRemove);
			lua_register(L, "_addComponent", Core::ECS::luaAddComponent);
			lua_register(L, "_getComponent", Core::ECS::luaGetComponent);
			lua_register(L, "_setScript", Core::Script::luaSetScript);
			lua_register(L, "_setPosition", Core::Transform::luaSetPosition);
			lua_register(L, "_setRotation", Core::Transform::luaSetRotation);
			lua_register(L, "_setScale", Core::Transform::luaSetScale);
			lua_register(L, "_getTransform", Core::Transform::luaGetTransform);
			lua_register(L, "_setModel", Core::ModelRenderer::luaSetModel);
			lua_register(L, "_setDirectionalLight", Core::DirectionalLight::luaSetDirectionalLight);
			lua_register(L, "_setPointLight", Core::PointLight::luaSetPointLight);
			lua_register(L, "_setRigidBody", Core::RigidBody::luaSetRigidBody);
			lua_register(L, "_rigidBodyApplyForce", Core::RigidBody::luaApplyForce);
			lua_register(L, "_rigidBodyColShapeSphere", Core::RigidBody::luaColShapeSphere);
			lua_register(L, "_rigidBodyColShapeBox", Core::RigidBody::luaColShapeBox);
			lua_register(L, "_rigidBodyColShapeCapsule", Core::RigidBody::luaColShapeCapsule);
			lua_register(L, "_toggleCursor", Core::Input::luaToggleCursor);
			lua_register(L, "_isCursorLocked", Core::Input::luaIsCursorLocked);
			lua_register(L, "_updateCameraPerspective", Core::GameEngine::luaUpdateCameraPerspective);
			lua_register(L, "_sceneDoPreset", Core::Scene::luaDoPreset);
			lua_register(L, "_getInt", getInt);
			lua_register(L, "_getFloat", getFloat);
			lua_register(L, "_sendMessage", sendMessage);
			lua_register(L, "_addWall", addWall);
			lua_register(L, "_setTextureSheet", setTextureSheet);
		}
		void pushAllGlobals(lua_State* L)
		{
			for (unsigned int i = 0; i < (unsigned int)Physics::CollisionShapeType::COUNT; i++)
			{
				const char* name = Physics::getCollisionShapeName((Physics::CollisionShapeType)i);
				lua_pushinteger(L, i);
				lua_setglobal(L, name);
			}

			//Add globals
			for (unsigned int i = 0; i < (unsigned int)ComponentType::COUNT; i++)
			{
				ComponentHint data = getComponentHint((ComponentType)i);
				lua_pushinteger(L, i);
				lua_setglobal(L, data.name);
			}


			for (unsigned int i = 0; i < (unsigned int)MessageType::COUNT; i++)
			{
				const auto& data = getMessageData((MessageType)i);
				lua_pushinteger(L, i);
				lua_setglobal(L, data.name);
			}

			for (unsigned int i = 0; i < InputCodes::NUM_KEYS; i++)
			{
				const char* name = InputCodes::toString(i);
				if (strlen(name) != 0)
				{
					lua_pushinteger(L, i);
					lua_setglobal(L, name);
				}
			}
		}
	}
}