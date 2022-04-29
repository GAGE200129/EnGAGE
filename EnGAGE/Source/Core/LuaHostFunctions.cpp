#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"
#include "InputCodes.hpp"
#include "Renderer.hpp"
#include "Resource.hpp"
#include "Script.hpp"
#include "Scene.hpp"
#include "Messenger.hpp"

#include <BulletDynamics/Dynamics/btRigidBody.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#ifdef EN_DEBUG
#define CHECK_NUM_ARGS(L, numArgs) checkNumArguments(L, numArgs, __FUNCTION__)
#define CHECK_ARG(L, index, type) checkArgument(L, index, type, __FUNCTION__)
#else
#define CHECK_NUM_ARGS(L, numArgs)
#define CHECK_ARG(L, index, type)
#endif

//ECS
int createEntity(lua_State* L);
int markForRemove(lua_State* L);
int addComponent(lua_State* L);
int getComponent(lua_State* L);


//Input
int toggleCursor(lua_State* L);
int isCursorLocked(lua_State* L);

//Transform
int setPosition(lua_State* L);
int setRotation(lua_State* L);
int setScale(lua_State* L);

//Modelrenderer
int setModel(lua_State* L);

//Ridgidbody
int setRigidBody(lua_State* L);

//Script
int setScript(lua_State* L);

//Render
int updateCamera(lua_State* L);
int setDirectionalLight(lua_State* L);

//Scene
int loadScene(lua_State* L);
int saveScene(lua_State* L);

//onMessageFunc
int getInt(lua_State* L);
int getFloat(lua_State* L);
int sendMessage(lua_State* L);

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


namespace LuaHostFunctions
{
	void registerAllScriptFunctions(lua_State* L)
	{
		lua_register(L, "_createEntity", createEntity);
		lua_register(L, "_addComponent", addComponent);
		lua_register(L, "_setScript", setScript);
		lua_register(L, "_markForRemove", markForRemove);
		lua_register(L, "_getComponent", getComponent);
		lua_register(L, "_toggleCursor", toggleCursor);
		lua_register(L, "_isCursorLocked", isCursorLocked);
		lua_register(L, "_setPosition", setPosition);
		lua_register(L, "_setRotation", setRotation);
		lua_register(L, "_setScale", setScale);
		lua_register(L, "_updateCamera", updateCamera);
		lua_register(L, "_getInt", getInt);
		lua_register(L, "_getFloat", getFloat);
		lua_register(L, "_sendMessage", sendMessage);
		lua_register(L, "_setModel", setModel);
		lua_register(L, "_setRigidBody", setRigidBody);
	}
	void registerAllSceneFunctions(lua_State* L)
	{
		lua_register(L, "_createEntity", createEntity);
		lua_register(L, "_markForRemove", markForRemove);
		lua_register(L, "_addComponent", addComponent);
		lua_register(L, "_getComponent", getComponent);
		lua_register(L, "_setDirectionalLight", setDirectionalLight);
		lua_register(L, "_setModel", setModel);
		lua_register(L, "_setRigidBody", setRigidBody);
		lua_register(L, "_setPosition", setPosition);
		lua_register(L, "_setRotation", setRotation);
		lua_register(L, "_setScale", setScale);
		lua_register(L, "_setScript", setScript);
	}

}

using namespace Core;

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
	DirectionalLightComponent* component = (DirectionalLightComponent*)header;
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
	CHECK_ARG(L, 1, LUA_TNUMBER);
	
	unsigned int numArgs = lua_gettop(L) - 1;
	Message message;

	message.type = (MessageType)lua_tointeger(L, 1);
	unsigned int offset = 0;
	for (unsigned int i = 1; i < numArgs; i++)
	{
		
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TNUMBER:
		{
			double value = (double)lua_tonumber(L, i);
			memcpy(message.message + offset, &value, sizeof(int));
			offset += sizeof(double);
			break;
		}
		}
	}

	Messenger::queueMessage(&message);

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
	Messenger::recieveMessage(MessageType::TOGGLE_CURSOR);
	return 0;
}
int isCursorLocked(lua_State* L)
{
	CHECK_NUM_ARGS(L, 0);
	auto message = Messenger::request(RequestType::CURSOR_LOCKED);
	int result = *reinterpret_cast<int*>(message.data);
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
	TransformComponent* transform = (TransformComponent*)header;
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
	TransformComponent* transform = (TransformComponent*)header;
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
	TransformComponent* transform = (TransformComponent*)header;
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

	ModelRendererComponent* pModelRenderer = (ModelRendererComponent*)header;
	pModelRenderer->pModel = Resource::getModel(modelName);
	strcpy(pModelRenderer->modelPath, modelName);
	return 0;
}
int setRigidBody(lua_State* L)
{
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

	RigidBodyComponent* pComponent = (RigidBodyComponent*)header;
	btRigidBody* pRigidBody = (btRigidBody*)pComponent->pRigidbody;
	pRigidBody->setMassProps((float)lua_tonumber(L, 2), pRigidBody->getLocalInertia());
	pComponent->collisionShapeType = (unsigned int)lua_tointeger(L, 3);

	unsigned int numArguments = lua_gettop(L) - 3;

	PhysicsInitCollisionShapeMessage message;

	message.body = pRigidBody;
	message.type = pComponent->collisionShapeType;

	EN_ASSERT(numArguments * sizeof(float) < 50, "Args overflow !");
	float* argData = (float*)message.arguments;
	for (unsigned int i = 0; i < numArguments; i++)
	{
		argData[i] = (float)lua_tonumber(L, i + 4);
	}

	Messenger::recieveMessage(MessageType::PHYSICS_INIT_COLLISION_SHAPE , &message);

	return 0;
}

int setScript(lua_State* L)
{
	EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::SCRIPT, "Invalid component type");

	const char* scriptPath = lua_tostring(L, 2);
	EN_ASSERT(scriptPath != nullptr, "scriptPath is null");

	ScriptComponent* sciptComponent = (ScriptComponent*)header;
	Script::loadFile(sciptComponent->L, scriptPath);
	strcpy(sciptComponent->scriptPath, scriptPath);
	return 0;
}
int updateCamera(lua_State* L)
{
	EN_ASSERT(lua_gettop(L) == 9, "Invalid argument");
	auto& camera = Core::Renderer::getCamera();
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