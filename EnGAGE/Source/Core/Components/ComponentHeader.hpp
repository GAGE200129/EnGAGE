#pragma once

#include "ComponentTypes.hpp"

namespace Core
{
	struct ComponentHeader
	{
		ComponentType type;
		UInt64 entity;
		void (*OnImGui)(ComponentHeader* pComponent);
		void (*OnSeralize)(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void (*Destroy)(ComponentHeader* pComponent);
	};
}