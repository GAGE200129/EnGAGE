#pragma once

#include "pch.hpp"

#include "Model.hpp"

namespace Core
{
	enum class ComponentType : unsigned int
	{
		TRANSFORM,
		MODEL_RENDERER,
		COUNT
	};

	struct ComponentHeader
	{
		unsigned int entity;
	};

	struct TransformComponent
	{
		ComponentHeader header;
		float x, y, z,
			rw, rx, ry, rz,
			sx, sy, sz;
	};

	struct ModelRendererComponent
	{
		ComponentHeader header;
		Ptr<Model> pModel;
	};

	inline static constexpr unsigned int sizeofComponent(const ComponentType& type)
	{
		switch (type)
		{
		case ComponentType::TRANSFORM:
			return sizeof(TransformComponent);
		case ComponentType::MODEL_RENDERER:
			return sizeof(ModelRendererComponent);
		default:
			EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
			return 0;
		}
	}

}