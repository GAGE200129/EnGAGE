#pragma once

struct lua_State;

namespace LuaHostFunctions
{
	//ECS
	int createEntity(lua_State* L);
	int markForRemove(lua_State* L);
	int addComponent(lua_State* L);
	int getComponent(lua_State* L);


	//Input
	int keyPressed(lua_State* L);
	int keyPressedOnce(lua_State* L);
	int buttonPressed(lua_State* L);
	int getCursorPosDelta(lua_State* L);
	int toggleCursor(lua_State* L);
	int isCursorLocked(lua_State* L);

	//Transform
	int setPosition(lua_State* L);
	int setRotation(lua_State* L);
	int setScale(lua_State* L);
	int translate(lua_State* L);
	int rotate(lua_State* L);
	int scale(lua_State* L);

	//Modelrenderer
	int setModel(lua_State* L);
	
	//Ridgidbody
	int setRigidBody(lua_State* L);
	int getVelocity(lua_State* L);
	int setCollisionShapeSphere(lua_State* L);

	//Script
	int setScript(lua_State* L);

	//Render
	int updateCamera(lua_State* L);
}