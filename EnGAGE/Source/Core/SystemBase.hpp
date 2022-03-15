#pragma once

#include "ComponentBase.hpp"
#include "pch.hpp"

namespace Core
{
	template<ComponentType type, typename DataType>
	class SystemBase
	{
	public:
		virtual ~SystemBase() = default;

		void addComponent(Ref<ComponentBase> component)
		{
			if (component->mType == type)
			{
				mComponents.push_back(std::static_pointer_cast<DataType>(component));
			}
		}

	protected:
		DynArr<Ref<DataType>> mComponents;
	};
}