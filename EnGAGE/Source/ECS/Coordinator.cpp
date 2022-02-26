#include "pch.hpp"
#include "Coordinator.hpp"

namespace ECS
{

	Entity Coordinator::createEntity()
	{
		return Entity();
	}

	void Coordinator::destroyEntity(Entity entity)
	{
		mEntityManager.destroyEntity(entity);
		mComponentManager.entityDestroyed(entity);
		mSystemManager.entityDestroyed(entity);
	}

}
