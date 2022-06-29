#pragma once

#include "Components.hpp"
#include "Systems.hpp"
#include "ECSConstants.hpp"

namespace Core::ECS
{
	//Entity 
	struct EntitySignature
	{
		UInt64 id;
		UInt32 signature : MAX_COMPONENTS;
	};


	void init();
	void clear();
	void updateRemovedEntities();
	UInt64 createEntity();
	void removeEntity(UInt64 entity);
	void markForRemove(UInt64 entity);
	const Arr<EntitySignature, MAX_ENTITIES>& getEntitySignatures();
	UInt64 getEntityCount();
	void* addComponent(UInt64 entity, ComponentType type);
	void removeComponent(UInt64 entity, ComponentType type);
	void* getComponent(UInt64 entity, ComponentType type);

	int luaCreateEntity(lua_State* L);
	int luaMarkForRemove(lua_State* L);
	int luaAddComponent(lua_State* L);
	int luaGetComponent(lua_State* L);

	System& getSystem(SystemType type);

	UInt64 getComponentArrayMemorySize(ComponentType type);

}