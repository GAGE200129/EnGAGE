#pragma once

#include "pch.hpp"

#include "ECS.hpp"
#include "ComponentArray.hpp"

namespace ECS
{
	class ComponentManager
	{

	public:
		ComponentManager() : mComponentTypes(), mComponentArrays(), mNextConponentType(0) {}

		template<typename T>
		void registerComponent()
		{
			const char* typeName = typeid(T).name();
			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

			mComponentTypes.insert({ typeName, mNextConponentType });
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
			mNextConponentType++;
		}

		template<typename T>
		ComponentType getComponentType()
		{
			const char* typeName = typeid(T).name();
			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			return mComponentTypes[typeName];
		}

		template<typename T>
		void addComponent(Entity e, T component)
		{
			getComponentArray<T>()->insertData(e, component);
		}

		template<typename T>
		void removeComponent(Entity e)
		{
			getComponentArray<T>()->removeData(e);
		}

		template<typename T>
		T& getComponent(Entity e)
		{
			return getComponentArray<T>()->getData(e);
		}


		void entityDestroyed(Entity e)
		{
			for (const auto& pair : mComponentArrays)
			{
				const auto& componentArr = pair.second;

				componentArr->entityDestroyed(e);
			}
		}
	private:
		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray()
		{
			const char* typeName = typeid(T).name();
			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
			
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}

	private:
		std::unordered_map<const char*, ComponentType> mComponentTypes;
		std::unordered_map<const char*, std::shared_ptr<IComponentArray> > mComponentArrays;
		ComponentType mNextConponentType;


	};
}