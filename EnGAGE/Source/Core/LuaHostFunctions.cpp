#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


int lua_getComponent(lua_State* L)
{
    using namespace Core::ECS;
    EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");

    unsigned int entityID = lua_tointeger(L, 1);
    ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

    void* component = getComponent(entityID, componentType);

    if (component == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }

    switch (componentType)
    {
    case Core::ECS::ComponentType::NAME:
    {
        NameComponent* nameComp = (NameComponent*)component;
        lua_pushstring(L, nameComp->name);
        return 1;
    }
    case Core::ECS::ComponentType::TRANSFORM:
    {
        TransformComponent* transform = (TransformComponent*)component;
        lua_newtable(L);
        lua_pushstring(L, "x");
        lua_pushnumber(L, transform->x);
        lua_settable(L, -3);
        lua_pushstring(L, "y");
        lua_pushnumber(L, transform->y);
        lua_settable(L, -3);
        lua_pushstring(L, "z");
        lua_pushnumber(L, transform->z);
        lua_settable(L, -3);

        lua_pushstring(L, "rw");
        lua_pushnumber(L, transform->rw);
        lua_settable(L, -3);
        lua_pushstring(L, "rx");
        lua_pushnumber(L, transform->rx);
        lua_settable(L, -3);
        lua_pushstring(L, "ry");
        lua_pushnumber(L, transform->ry);
        lua_settable(L, -3);
        lua_pushstring(L, "rz");
        lua_pushnumber(L, transform->rz);
        lua_settable(L, -3);

        lua_pushstring(L, "sx");
        lua_pushnumber(L, transform->sx);
        lua_settable(L, -3);
        lua_pushstring(L, "sy");
        lua_pushnumber(L, transform->sy);
        lua_settable(L, -3);
        lua_pushstring(L, "sz");
        lua_pushnumber(L, transform->sz);
        lua_settable(L, -3);
        return 1;
    }
    }

    lua_pushnil(L);
    return 1;
}

int lua_updateComponent(lua_State* L)
{
    using namespace Core::ECS;
    EN_ASSERT(lua_gettop(L) == 3, "Invalid argument");

    unsigned int entityID = lua_tointeger(L, 1);
    ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

    void* component = getComponent(entityID, componentType);
    if (component == nullptr)
    {
        return 0;
    }

    switch (componentType)
    {
    case Core::ECS::ComponentType::NAME:
    {
        NameComponent* nameComp = (NameComponent*)component;
        const char* name = lua_tostring(L, 3);
        strcpy(nameComp->name, name);
        break;
    }
    case Core::ECS::ComponentType::TRANSFORM:
    {
        TransformComponent* transform = (TransformComponent*)component;

        lua_getfield(L, -1, "x");
        transform->x = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        transform->y = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "z");
        transform->z = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "rw");
        transform->rw = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "rx");
        transform->rx = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "ry");
        transform->ry = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "rz");
        transform->rz = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "sx");
        transform->sx = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "sy");
        transform->sy = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "sz");
        transform->sz = lua_tonumber(L, -1);
        lua_pop(L, 1);
        break;
    }
    }

    return 0;
}

