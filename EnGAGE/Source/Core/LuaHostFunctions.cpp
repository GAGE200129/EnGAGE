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
#include "Core/ECS/KinematicBody.hpp"
#include "Core/ECS/Transform.hpp"
#include "Core/ECS/ModelRenderer.hpp"
#include "Core/ECS/Script.hpp"
#include "Core/ECS/DirectionalLight.hpp"
#include "Core/ECS/PointLight.hpp"
#include "Core/ECS/Name.hpp"
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
			lua_register(L, "_kinematicBodySetVelocity", Core::KinematicBody::luaSetVelocity);
			lua_register(L, "_kinematicBodyJump", Core::KinematicBody::luaJump);
			lua_register(L, "_toggleCursor", Core::Input::luaToggleCursor);
			lua_register(L, "_isCursorLocked", Core::Input::luaIsCursorLocked);
			lua_register(L, "_updateCameraPerspective", Core::GameEngine::luaUpdateCameraPerspective);
			lua_register(L, "_getInt", getInt);
			lua_register(L, "_getFloat", getFloat);
			lua_register(L, "_sendMessage", sendMessage);
			lua_register(L, "_addFace", Core::Map::luaAddFace);
			lua_register(L, "_setTextureSheet", Core::Map::luaSetTextureSheet);
			lua_register(L, "_setName", Core::Name::luaSetName);
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