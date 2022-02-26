#pragma once

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

namespace ECS
{
	class Coordinator
	{
	public:
		Coordinator() : mComponentManager(), mEntityManager(), mSystemManager() {}
		// Entity methods
		Entity createEntity();
		void destroyEntity(Entity entity);

		template<typename T>
		void registerComponent()
		{
			mComponentManager.registerComponent<T>();
		}

		template<typename T>
		void addComponent(Entity entity, T component)
		{
			mComponentManager.addComponent<T>(entity, component);

			auto signature = mEntityManager.getSignature(entity);
			signature.set(mComponentManager.getComponentType<T>(), true);
			mEntityManager.setSignature(entity, signature);

			mSystemManager.entitySignatureChanged(entity, signature);
		}

		template<typename T>
		void removeComponent(Entity entity)
		{
			mComponentManager.removeComponent<T>(entity);

			auto signature = mEntityManager.getSignature(entity);
			signature.set(mComponentManager.getComponentType<T>(), false);
			mEntityManager.setSignature(entity, signature);

			mSystemManager.entitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& getComponent(Entity entity)
		{
			return mComponentManager.getComponent<T>(entity);
		}

		template<typename T>
		ComponentType getComponentType()
		{
			return mComponentManager.getComponentType<T>();
		}

		// System methods
		template<typename T>
		std::shared_ptr<T> registerSystem()
		{
			return mSystemManager.registerSystem<T>();
		}

		template<typename T>
		void setSystemSignature(Signature signature)
		{
			mSystemManager.setSignature<T>(signature);
		}


	private:
		ComponentManager mComponentManager;
		EntityManager mEntityManager;
		SystemManager mSystemManager;
	};
}