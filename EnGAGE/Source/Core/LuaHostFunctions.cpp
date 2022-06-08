#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"
#include "InputCodes.hpp"
#include "Renderer/Renderer.hpp"
#include "Resource.hpp"
#include "Scripting.hpp"
#include "Scene.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Physics.hpp"
#include "Input.hpp"
#include "GameEngine.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/Script.hpp"
#include "Components/DirectionalLight.hpp"
#include "Components/PointLight.hpp"
#include "Map/Map.hpp"

#include <BulletDynamics/Dynamics/btRigidBody.h>

#ifdef EN_DEBUG
#define CHECK_NUM_ARGS(L, numArgs) checkNumArguments(L, numArgs, __FUNCTION__)
#define CHECK_ARG(L, index, type) checkArgument(L, index, type, __FUNCTION__)
#else
#define CHECK_NUM_ARGS(L, numArgs)
#define CHECK_ARG(L, index, type)
#endif

namespace Core
{
	static void checkNumArguments(lua_State* L, unsigned int arguments, const char* functionName)
	{
		lua_Debug ar;
		lua_getstack(L, 1, &ar);
		lua_getinfo(L, "nSl", &ar);
		int line = ar.currentline;
		const char* fileName = ar.source;

		unsigned int numArguments = lua_gettop(L);
		EN_ASSERT(numArguments == arguments, "In file: {}, calling lua's host function: {}, line: {}, Invalid numbers of argument: {}, expected: {}", fileName, functionName, line, numArguments, arguments);
	}

	static void checkArgument(lua_State* L, int index, unsigned int expectedType, const char* functionName)
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

	int setPointLight(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 8);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);
		CHECK_ARG(L, 5, LUA_TNUMBER);
		CHECK_ARG(L, 6, LUA_TNUMBER);
		CHECK_ARG(L, 7, LUA_TNUMBER);
		CHECK_ARG(L, 8, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::POINT_LIGHT, "Invalid component type");
		PointLight::Component* component = (PointLight::Component*)header;
		component->color.x = (float)lua_tonumber(L, 2);
		component->color.y = (float)lua_tonumber(L, 3);
		component->color.z = (float)lua_tonumber(L, 4);
		component->intensity = (float)lua_tonumber(L, 5);

		component->constant = (float)lua_tonumber(L, 5);
		component->linear = (float)lua_tonumber(L, 6);
		component->exponent = (float)lua_tonumber(L, 7);


		return 0;
	}

	int setDirectionalLight(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 8);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);
		CHECK_ARG(L, 5, LUA_TNUMBER);
		CHECK_ARG(L, 6, LUA_TNUMBER);
		CHECK_ARG(L, 7, LUA_TNUMBER);
		CHECK_ARG(L, 8, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::DIRECTIONAL_LIGHT, "Invalid component type");
		DirectionalLight::Component* component = (DirectionalLight::Component*)header;
		component->direction.x = (float)lua_tonumber(L, 2);
		component->direction.y = (float)lua_tonumber(L, 3);
		component->direction.z = (float)lua_tonumber(L, 4);

		component->color.x = (float)lua_tonumber(L, 5);
		component->color.y = (float)lua_tonumber(L, 6);
		component->color.z = (float)lua_tonumber(L, 7);

		component->intensity = (float)lua_tonumber(L, 8);

		return 0;
	}

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

	int createEntity(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		lua_pushinteger(L, ECS::createEntity());
		return 1;
	}

	int markForRemove(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 1);
		CHECK_ARG(L, 1, LUA_TNUMBER);

		unsigned int entity = (unsigned int)lua_tointeger(L, 1);
		ECS::markForRemove(entity);
		return 0;
	}

	int addComponent(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TNUMBER);// Entity id
		CHECK_ARG(L, 2, LUA_TNUMBER);// Type

		unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
		ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

		void* data = ECS::addComponent(entityID, componentType);
		lua_pushlightuserdata(L, data);
		return 1;
	}

	int getComponent(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TNUMBER);// Entity id
		CHECK_ARG(L, 2, LUA_TNUMBER);// Type

		unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
		ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

		void* component = ECS::getComponent(entityID, componentType);

		if (component == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(L, component);
		return 1;
	}

	int toggleCursor(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		Input::toggleCursor();
		return 0;
	}
	int isCursorLocked(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 0);
		int result = Input::cursorLocked();
		lua_pushboolean(L, result);
		return 1;
	}


	int setPosition(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 4);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
		Transform::Component* transform = (Transform::Component*)header;
		transform->x = (float)lua_tonumber(L, 2);
		transform->y = (float)lua_tonumber(L, 3);
		transform->z = (float)lua_tonumber(L, 4);
		return 0;
	}

	int setRotation(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 5);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);
		CHECK_ARG(L, 5, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
		Transform::Component* transform = (Transform::Component*)header;
		transform->rw = (float)lua_tonumber(L, 2);
		transform->rx = (float)lua_tonumber(L, 3);
		transform->ry = (float)lua_tonumber(L, 4);
		transform->rz = (float)lua_tonumber(L, 5);
		return 0;
	}

	int setScale(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 4);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
		Transform::Component* transform = (Transform::Component*)header;
		transform->sx = (float)lua_tonumber(L, 2);
		transform->sy = (float)lua_tonumber(L, 3);
		transform->sz = (float)lua_tonumber(L, 4);
		return 0;
	}


	int setModel(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TSTRING);


		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::MODEL_RENDERER, "Invalid component type");

		const char* modelName = lua_tostring(L, 2);
		EN_ASSERT(modelName != nullptr, "modelName is null");

		ModelRenderer::Component* pModelRenderer = (ModelRenderer::Component*)header;
		pModelRenderer->pModel = Resource::getModel(modelName);
		strcpy((char*)pModelRenderer->modelPath, modelName);
		return 0;
	}
	int setRigidBody(lua_State* L)
	{
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

		RigidBody::Component* pComponent = (RigidBody::Component*)header;
		btRigidBody* pRigidBody = (btRigidBody*)pComponent->pRigidbody;
		pRigidBody->setMassProps((float)lua_tonumber(L, 2), pRigidBody->getLocalInertia());

		return 0;
	}

	int setScript(lua_State* L)
	{
		EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::SCRIPT, "Invalid component type");

		const char* scriptPath = lua_tostring(L, 2);
		EN_ASSERT(scriptPath != nullptr, "scriptPath is null");

		Script::Component* sciptComponent = (Script::Component*)header;
		Scripting::loadFile(sciptComponent->L, scriptPath);
		strcpy((char*)sciptComponent->scriptPath, scriptPath);
		return 0;
	}
	int updateCameraPerspective(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 9);
		auto& camera = Core::GameEngine::getMainCamera();
		camera.mode = Camera::Mode::PERSPECTIVE;
		camera.x = (float)lua_tonumber(L, 1);
		camera.y = (float)lua_tonumber(L, 2);
		camera.z = (float)lua_tonumber(L, 3);
		camera.pitch = (float)lua_tonumber(L, 4);
		camera.yaw = (float)lua_tonumber(L, 5);
		camera.roll = (float)lua_tonumber(L, 6);
		camera.fov = (float)lua_tonumber(L, 7);
		camera.near = (float)lua_tonumber(L, 8);
		camera.far = (float)lua_tonumber(L, 9);

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
	int setColShapeSphere(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 2);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

		RigidBody::Component* pComponent = (RigidBody::Component*)header;
		pComponent->collisionShapeType = (UInt8)Physics::CollisionShapeType::SPHERE;
		Physics::initColShapeSphere(pComponent->pRigidbody, (F32)lua_tonumber(L, 2));

		return 0;
	}

	int setColShapeBox(lua_State* L)
	{
		CHECK_NUM_ARGS(L, 4);
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		CHECK_ARG(L, 2, LUA_TNUMBER);
		CHECK_ARG(L, 3, LUA_TNUMBER);
		CHECK_ARG(L, 4, LUA_TNUMBER);

		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

		RigidBody::Component* pComponent = (RigidBody::Component*)header;
		pComponent->collisionShapeType = (UInt8)Physics::CollisionShapeType::BOX;
		Physics::initColShapeBox(pComponent->pRigidbody, (F32)lua_tonumber(L, 2), (F32)lua_tonumber(L, 3), (F32)lua_tonumber(L, 4));

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

	int getTransform(lua_State* L)
	{
		CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
		ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
		EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
		Transform::Component* pTransform = (Transform::Component*)header;

		lua_pushnumber(L, pTransform->x);
		lua_pushnumber(L, pTransform->y);
		lua_pushnumber(L, pTransform->z);

		lua_pushnumber(L, pTransform->rw);
		lua_pushnumber(L, pTransform->rx);
		lua_pushnumber(L, pTransform->ry);
		lua_pushnumber(L, pTransform->rz);

		lua_pushnumber(L, pTransform->sx);
		lua_pushnumber(L, pTransform->sy);
		lua_pushnumber(L, pTransform->sz);
		return 10;
	}
	namespace LuaHostFunctions
	{
		void registerFunctions(lua_State* L)
		{
			lua_register(L, "_createEntity", createEntity);
			lua_register(L, "_markForRemove", markForRemove);
			lua_register(L, "_addComponent", addComponent);
			lua_register(L, "_setScript", setScript);
			lua_register(L, "_setPosition", setPosition);
			lua_register(L, "_setRotation", setRotation);
			lua_register(L, "_setScale", setScale);
			lua_register(L, "_setModel", setModel);
			lua_register(L, "_setDirectionalLight", setDirectionalLight);
			lua_register(L, "_setPointLight", setPointLight);
			lua_register(L, "_setRigidBody", setRigidBody);
			lua_register(L, "_setColShapeSphere", setColShapeSphere);
			lua_register(L, "_setColShapeBox", setColShapeBox);
			lua_register(L, "_getComponent", getComponent);
			lua_register(L, "_toggleCursor", toggleCursor);
			lua_register(L, "_isCursorLocked", isCursorLocked);
			lua_register(L, "_updateCameraPerspective", updateCameraPerspective);
			lua_register(L, "_getInt", getInt);
			lua_register(L, "_getFloat", getFloat);
			lua_register(L, "_sendMessage", sendMessage);
			lua_register(L, "_addWall", addWall);
			lua_register(L, "_setTextureSheet", setTextureSheet);
			lua_register(L, "_getTransform", getTransform);
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