#include "pch.hpp"
#include "EntityManager.hpp"

namespace ECS
{

	EntityManager::EntityManager() :
		mAvailableEntities(),
		mSignatures(),
		mLivingEntityCount(0)
	{
		for (Entity e = 0; e < MAX_ENTITIES; e++)
		{
			mAvailableEntities.push(e);
		}
	}

	Entity EntityManager::createEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities !");

		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		mLivingEntityCount++;

		return id;
	}

	void EntityManager::destroyEntity(Entity e)
	{
		assert(e < MAX_ENTITIES && "Entity out of range !");

		mSignatures[e].reset();

		mAvailableEntities.push(e);
		mLivingEntityCount--;

	}

	void EntityManager::setSignature(Entity e, const Signature& signature)
	{
		assert(e < MAX_ENTITIES && "Entity out of range !");
		mSignatures[e] = signature;
	}

	Signature EntityManager::getSignature(Entity e)
	{
		assert(e < MAX_ENTITIES && "Entity out of range !");
		return mSignatures[e];
	}

}
