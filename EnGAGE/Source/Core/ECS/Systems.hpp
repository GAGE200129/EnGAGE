#pragma once
#include "ECSConstants.hpp"

namespace Core
{
	//System
	enum class SystemType : unsigned int
	{
		RENDERER,
		DIRECTIONAL,
		POINT,
		SCRIPTING,
		PHYSICS,
		KINEMATIC,
		ANIMATION,
		COUNT
	};
	struct System
	{
		unsigned int signature : ECS::MAX_COMPONENTS;
		Set<UInt64> entities;
	};

}