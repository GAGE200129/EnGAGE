#pragma once
#include "ComponentHeader.hpp"
#include "Core/ECSConstants.hpp"
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
		inline static ComponentHint getHint()
		{
			return ComponentHint{ "RIGID_BODY", sizeof(Component) };
		}
	}
}