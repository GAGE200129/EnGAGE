#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"
#include "InputCodes.hpp"
#include "Input.hpp"
#include "Renderer.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace LuaHostFunctions
{
    int keyPressed(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        uint16_t keyCode = (uint16_t)lua_tointeger(L, 1);
        lua_pushboolean(L, Core::Input::isKeyPressed(keyCode));
        return 1;
    }
    int keyPressedOnce(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        uint16_t keyCode = (uint16_t)lua_tointeger(L, 1);
        lua_pushboolean(L, Core::Input::isKeyPressedOnce(keyCode));
        return 1;
    }
    int buttonPressed(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        uint16_t keyCode = (uint16_t)lua_tointeger(L, 1);
        lua_pushboolean(L, Core::Input::isButtonPressed(keyCode));
        return 1;
    }
    int getCursorPosDelta(lua_State* L)
    {
        lua_pushnumber(L, Core::Input::getCursorDX());
        lua_pushnumber(L, Core::Input::getCursorDY());
        return 2;
    }
    int toggleCursor(lua_State* L)
    {
        Core::Input::toggleCursor();
        return 0;
    }
    int isCursorLocked(lua_State* L)
    {
        lua_pushboolean(L, Core::Input::cursorLocked());
        return 1;
    }
    int getComponent(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");

        unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
        ComponentType componentType = (ComponentType)lua_tointeger(L, 2);

        void* component = getComponent(entityID, componentType);

        if (component == nullptr)
        {
            lua_pushnil(L);
            return 1;
        }

        lua_pushlightuserdata(L, component);
        return 1;
    }

    int setPosition(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 4, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->x = (float)lua_tonumber(L, 2);
        transform->y = (float)lua_tonumber(L, 3);
        transform->z = (float)lua_tonumber(L, 4);
        return 0;
    }

    int setRotation(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 5, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->rw = (float)lua_tonumber(L, 2);
        transform->rx = (float)lua_tonumber(L, 3);
        transform->ry = (float)lua_tonumber(L, 4);
        transform->rz = (float)lua_tonumber(L, 5);
        return 0;
    }

    int setScale(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 4, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->sx = (float)lua_tonumber(L, 2);
        transform->sy = (float)lua_tonumber(L, 3);
        transform->sz = (float)lua_tonumber(L, 4);
        return 0;
    }

    int translate(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 4, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->x += (float)lua_tonumber(L, 2);
        transform->y += (float)lua_tonumber(L, 3);
        transform->z += (float)lua_tonumber(L, 4);
        return 0;
    }

    int rotate(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 5, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->rw += (float)lua_tonumber(L, 2);
        transform->rx += (float)lua_tonumber(L, 3);
        transform->ry += (float)lua_tonumber(L, 4);
        transform->rz += (float)lua_tonumber(L, 5);
        return 0;
    }

    int scale(lua_State* L)
    {
        using namespace Core::ECS;
        EN_ASSERT(lua_gettop(L) == 4, "Invalid argument");

        ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == ComponentType::TRANSFORM, "Invalid component type");
        TransformComponent* transform = (TransformComponent*)header;
        transform->sx += (float)lua_tonumber(L, 2);
        transform->sy += (float)lua_tonumber(L, 3);
        transform->sz += (float)lua_tonumber(L, 4);
        return 0;
    }
    int updateCamera(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 9, "Invalid argument");
        auto& camera = Core::Renderer::getCamera();
        camera.x = (float)lua_tonumber(L, 1);
        camera.y = (float)lua_tonumber(L, 2);
        camera.z = (float)lua_tonumber(L, 3);
        camera.pitch =  (float)lua_tonumber(L, 4);
        camera.yaw =    (float)lua_tonumber(L, 5);
        camera.roll =   (float)lua_tonumber(L, 6);
        camera.fov =    (float)lua_tonumber(L, 7);
        camera.near =   (float)lua_tonumber(L, 8);
        camera.far =    (float)lua_tonumber(L, 9);

        return 0;
    }
}

