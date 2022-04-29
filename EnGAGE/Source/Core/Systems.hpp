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
		COUNT
	};
	struct System
	{
		unsigned int signature : ECS::MAX_COMPONENTS;
		Set<unsigned int> entities;
	};

}