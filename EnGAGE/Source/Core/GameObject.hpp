#pragma once

#include "pch.hpp"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Core
{
	class ComponentBase;
	class GameObject
	{
	public:

		template<typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			mComponents.push_back(createRef<T>(args...));
		}

	public:
		DynArr<Ref<ComponentBase>> mComponents;
		glm::vec3 mPosition = { 0, 0, 0 };
		glm::vec3 mScale = { 1, 1, 1 };
		glm::quat mRotation = { 1, 0, 0, 0 };
	};
}