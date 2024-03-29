#pragma once

namespace Core::ECS
{
	static constexpr UInt64 MAX_ENTITIES = 3000;
	static constexpr UInt64 MAX_COMPONENTS = 32; //32 bits field
	static constexpr UInt64 MAX_COMPONENT_ARRAY_BUFFER_SIZE = 1048576u;
	static constexpr UInt8 MAX_NAME_SIZE = 70u;
	static constexpr UInt8 MAX_BONE_MATRICES = 100u;
}