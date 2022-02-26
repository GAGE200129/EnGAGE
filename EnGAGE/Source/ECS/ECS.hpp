#pragma once

#include "pch.hpp"

namespace ECS
{
	using ComponentType = uint8_t;//max 256 type of components
	const uint8_t MAX_COMPONENTS = 32; //Per entity
	using Entity = uint32_t;
	static constexpr uint32_t MAX_ENTITIES = 5000;
	using Signature = std::bitset<MAX_COMPONENTS>;
}