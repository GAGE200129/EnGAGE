#pragma once

#include "pch.hpp"
#include "ECS.hpp"

namespace ECS
{

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void entityDestroyed(Entity e) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray() : mComponentArray(), mSize(0), mEntityToIndex(), mIndexToEntity() {};

		inline void insertData(Entity e, T component)
		{
			assert(mEntityToIndex.find(e) == mEntityToIndex.end() && "Component added to same entity more than once.");

			// Put new entry at end and update the maps

			size_t newIndex = mSize;
			mEntityToIndex[e] = newIndex;
			mIndexToEntity[newIndex] = e;
			mComponentArray[newIndex] = component;
			mSize++;
		}

		inline void removeData(Entity e)
		{
			assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Component not found.");
			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = mEntityToIndex[e];
			size_t indexOfLastElement = mSize - 1;
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];
			mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

			mEntityToIndex.erase(e);
			mIndexToEntity.erase(indexOfLastElement);

			mSize--;

		}

		T& getData(Entity e)
		{
			assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Retrieving non-existent component.");

			// Return a reference to the entity's component
			return mComponentArray[mEntityToIndex[e]];
		}

		void entityDestroyed(Entity e) override 
		{
			if (mEntityToIndex.find(e) != mEntityToIndex.end())
			{
				// Remove the entity's component if it existed
				removeData(e);
			}
		};
	private:
		std::array<T, MAX_ENTITIES> mComponentArray;
		size_t mSize;
		std::unordered_map<Entity, size_t> mEntityToIndex;
		std::unordered_map<size_t, Entity> mIndexToEntity;

	};
}