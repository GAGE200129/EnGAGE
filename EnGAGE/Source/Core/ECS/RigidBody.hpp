#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"

class btRigidBody;
namespace Core
{
	namespace RigidBody
	{
		struct Component
		{
			ComponentHeader header;
			UInt8 collisionShapeType;
			btRigidBody* pRigidbody;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void Destroy(ComponentHeader* pHeader);
		void Init(ComponentHeader* pHeader);

		int luaSetRigidBody(lua_State* L);
		int luaApplyForce(lua_State* L);
		int luaColShapeSphere(lua_State* L);
		int luaColShapeBox(lua_State* L);
		int luaColShapeCapsule(lua_State* L);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "RIGID_BODY", sizeof(Component) };
		}
	}
}