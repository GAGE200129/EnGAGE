#pragma once


namespace Core
{
	enum class ComponentType
	{
		UNKNOWN,
		MODEL_RENDERER
	};

	class GameObject;
	class ComponentBase
	{
	public:
		ComponentBase(ComponentType type) : mType(type) {}
		virtual ~ComponentBase() = default;
	public:
		ComponentType mType;
		Ref<GameObject> mParent;
	};
}