#include "pch.hpp"
#include "KinematicBody.hpp"

#include "Core/Physics/Physics.hpp"
#include "Core/LuaHostFunctions.hpp"

#include <BulletDynamics/Character/btKinematicCharacterController.h>

void Core::KinematicBody::OnImGui(ComponentHeader* pHeader)
{
	using namespace Physics;
	Component* pComponent = (Component*)pHeader;
	btRigidBody* pRigidBody = pComponent->pKinematicBody;


}

void Core::KinematicBody::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
	out << "_addComponent(" << entity << ", " << getHint().name << ")\n";
	//KinematicBody::Component* component = (KinematicBody::Component*)pComponent;
	//out << "_setKinematicBody(" << componentName << ", " << component->pRigidbody->getMass() << ")\n";

}

void Core::KinematicBody::Destroy(ComponentHeader* pHeader)
{
	KinematicBody::Component* component = (KinematicBody::Component*)pHeader;
	if (component)
	{
		Physics::removeRigidBody(component->pKinematicBody);
	}
}

void Core::KinematicBody::Init(ComponentHeader* pHeader)
{
	KinematicBody::Component* component = (KinematicBody::Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->header.Destroy = Destroy;
	component->pKinematicBody = Physics::newKinematicBody(pHeader->entity, 1, 2);
	component->capsuleRadius = 1;
	component->capsuleHeight = 2;
	component->moveDir = { 0, 0, 0 };
	component->speedDamping = 0.1f;
	component->onGround = false;
	component->accelration = 7.0;
	component->maxSpeed = 10;
	component->jump = false;
	component->jumpDir = { 0, 0, 0 };
}

int Core::KinematicBody::luaSetVelocity(lua_State* L)
{
	CHECK_NUM_ARGS(L, 4);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::KINEMATIC_BODY, "Invalid component type");

	KinematicBody::Component* pComponent = (KinematicBody::Component*)header;
	pComponent->moveDir.setX(lua_tonumber(L, 2));
	pComponent->moveDir.setY(lua_tonumber(L, 3));
	pComponent->moveDir.setZ(lua_tonumber(L, 4));

	return 0;
}

int Core::KinematicBody::luaJump(lua_State* L)
{
	CHECK_NUM_ARGS(L, 4);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);
	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::KINEMATIC_BODY, "Invalid component type");
	KinematicBody::Component* pComponent = (KinematicBody::Component*)header;

	pComponent->jump = true;
	pComponent->jumpDir = btVector3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
	if (pComponent->jumpDir.fuzzyZero()) {
		pComponent->jumpDir.setValue(0, 1, 0);
	}
	pComponent->jumpDir.normalize();

	return 0;
}

//int Core::KinematicBody::luaSetRigidBody(lua_State* L)
//{
//	CHECK_NUM_ARGS(L, 2);
//	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
//	CHECK_ARG(L, 2, LUA_TNUMBER);
//
//	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
//	EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");
//
//	RigidBody::Component* pComponent = (RigidBody::Component*)header;
//	btRigidBody* pRigidBody = (btRigidBody*)pComponent->pRigidbody;
//	pRigidBody->setMassProps((float)lua_tonumber(L, 2), pRigidBody->getLocalInertia());
//
//	return 0;
//}

