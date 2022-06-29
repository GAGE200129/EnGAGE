#include "pch.hpp"
#include "RigidBody.hpp"

#include "Core/Physics/Physics.hpp"
#include "Core/LuaHostFunctions.hpp"

#include <BulletDynamics/Character/btKinematicCharacterController.h>

void Core::RigidBody::OnImGui(ComponentHeader* pHeader)
{
	using namespace Physics;
	Component* pComponent = (Component*)pHeader;
	btRigidBody* pRigidBody = pComponent->pRigidbody;

	if (pComponent->collisionShapeType != (unsigned int)CollisionShapeType::EMPTY)
	{
		float mass = pRigidBody->getMass();
		if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 500.0f))
		{
			btVector3 inertia;
			pRigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
			pRigidBody->setMassProps(mass, inertia);
			pRigidBody->updateInertiaTensor();
			pRigidBody->activate();

			Physics::updateRigidBody(pRigidBody);
		}

		btVector3 btVel = pRigidBody->getLinearVelocity();
		float vel[] = { btVel.x(), btVel.y(), btVel.z() };
		if (ImGui::DragFloat3("Velocity", vel, 0.1f))
		{
			pRigidBody->setLinearVelocity(btVector3(vel[0], vel[1], vel[2]));
			pRigidBody->activate();
		}
	}
	const char* currentColliderName = getCollisionShapeName((CollisionShapeType)pComponent->collisionShapeType);
	if (ImGui::BeginCombo("Collision shape type", currentColliderName))
	{
		for (unsigned int i = 0; i < (unsigned int)CollisionShapeType::COUNT; i++)
		{
			const char* colliderName = getCollisionShapeName((CollisionShapeType)i);
			if (ImGui::Selectable(colliderName))
			{
				pComponent->collisionShapeType = i;
				initCollisionShapeRuntime(pRigidBody, (CollisionShapeType)i);
			}
		}
		ImGui::EndCombo();
	}
}

void Core::RigidBody::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
	String componentName = entity + "_rigidBody";
	out << componentName << " = _addComponent(" << entity << ", " << getHint().name << ")\n";
	RigidBody::Component* component = (RigidBody::Component*)pComponent;
	out << "_setRigidBody(" << componentName << ", " << component->pRigidbody->getMass() << ")\n";

	auto rigidBody = component->pRigidbody;
	switch ((Physics::CollisionShapeType)component->collisionShapeType)
	{
	case Physics::CollisionShapeType::BOX:
	{
		btBoxShape* shape = (btBoxShape*)(rigidBody->getCollisionShape());
		btVector3 halfExtend = shape->getHalfExtentsWithoutMargin();
		out  << "_rigidBodyColShapeBox(" << componentName  << ", " << halfExtend.x() << ", " << halfExtend.y() << ", " << halfExtend.z() << ")\n";
		break;
	}
	case Physics::CollisionShapeType::SPHERE:
	{
		btSphereShape* shape = (btSphereShape*)(rigidBody->getCollisionShape());
		float radius = shape->getRadius();
		out  <<  "_rigidBodyColShapeSphere(" << componentName << ", " << radius << ")\n";
		break;
	}
	}
}

void Core::RigidBody::Destroy(ComponentHeader* pHeader)
{
	RigidBody::Component* component = (RigidBody::Component*)pHeader;
	if (component)
	{
		Physics::removeRigidBody(component->pRigidbody);
	}
}

void Core::RigidBody::Init(ComponentHeader* pHeader)
{
	RigidBody::Component* component = (RigidBody::Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->header.Destroy = Destroy;
	component->pRigidbody = Physics::newRigidBody(pHeader->entity);
	component->collisionShapeType = 0;
}

int Core::RigidBody::luaSetRigidBody(lua_State* L)
{
	CHECK_NUM_ARGS(L, 2);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

	RigidBody::Component* pComponent = (RigidBody::Component*)header;
	btRigidBody* pRigidBody = (btRigidBody*)pComponent->pRigidbody;
	pRigidBody->setMassProps((float)lua_tonumber(L, 2), pRigidBody->getLocalInertia());

	return 0;
}

int Core::RigidBody::luaApplyForce(lua_State* L)
{
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);
	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

	RigidBody::Component* pBody = (RigidBody::Component*)header;
	if (pBody->pRigidbody)
	{
		pBody->pRigidbody->applyCentralForce(btVector3(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4)));
		pBody->pRigidbody->activate();
	}


	return 0;
}

int Core::RigidBody::luaColShapeSphere(lua_State* L)
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

int Core::RigidBody::luaColShapeBox(lua_State* L)
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

int Core::RigidBody::luaColShapeCapsule(lua_State* L)
{
	CHECK_NUM_ARGS(L, 3);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::RIGID_BODY, "Invalid component type");

	RigidBody::Component* pComponent = (RigidBody::Component*)header;
	pComponent->collisionShapeType = (UInt8)Physics::CollisionShapeType::CAPSULE;
	Physics::initColShapeCapsule(pComponent->pRigidbody, (F32)lua_tonumber(L, 2), (F32)lua_tonumber(L, 3));

	return 0;
}
