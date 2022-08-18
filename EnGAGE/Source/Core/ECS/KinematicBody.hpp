#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"

class btRigidBody;
namespace Core
{
	namespace KinematicBody
	{
		struct Component
		{
			ComponentHeader header;
			btRigidBody* pKinematicBody;
			btScalar capsuleRadius;
			btScalar capsuleHeight;
			btVector3 moveDir;
			btScalar speedDamping;
			bool onGround;
			btScalar accelration; // (m / s^2)
			btScalar maxSpeed; // (m / s)

			bool jump;
			btVector3 jumpDir;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void Destroy(ComponentHeader* pHeader);
		void Init(ComponentHeader* pHeader);

		int luaSetVelocity(lua_State* L);
		int luaJump(lua_State* L);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "KINEMATIC_BODY", sizeof(Component) };
		}
	}
}