#include "pch.hpp"
#include "SystemManager.hpp"

void ECS::SystemManager::entityDestroyed(Entity entity)
{
	// Erase a destroyed entity from all system lists
	// mEntities is a set so no check needed
	for (auto const& pair : mSystems)
	{
		auto const& system = pair.second;

		system->mEntities.erase(entity);
	}
}

void ECS::SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature)
{
	// Notify each system that an entity's signature changed
	for (auto const& pair : mSystems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = mSignatures[type];

		// Entity signature matches system signature - insert into set
		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->mEntities.insert(entity);
		}
		// Entity signature does not match system signature - erase from set
		else
		{
			system->mEntities.erase(entity);
		}
	}
}
