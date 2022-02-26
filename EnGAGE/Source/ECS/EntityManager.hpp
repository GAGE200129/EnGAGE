#pragma once

#include "ECS.hpp"

namespace ECS
{

	class EntityManager
	{
	public:
		EntityManager();

		Entity createEntity();
		void destroyEntity(Entity e);

		void setSignature(Entity e, const Signature& signature);
		Signature getSignature(Entity e);
	private:
		std::queue<Entity> mAvailableEntities; // Queue of unused entity id
		std::array<Signature, MAX_ENTITIES> mSignatures; // Index = entity id
		uint32_t mLivingEntityCount;
	};
}