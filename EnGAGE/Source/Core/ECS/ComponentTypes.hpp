#pragma once


namespace Core
{
	enum class ComponentType : UInt8
	{
		NAME,
		TRANSFORM,
		MODEL_RENDERER,
		SCRIPT,
		RIGID_BODY,
		KINEMATIC_BODY,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		COUNT
	};
}