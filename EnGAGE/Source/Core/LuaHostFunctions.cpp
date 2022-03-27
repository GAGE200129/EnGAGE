#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"
#include "InputCodes.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Resource.hpp"
#include "Lua.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace LuaHostFunctions
{
    int createEntity(lua_State* L)
    {
        lua_pushinteger(L, Core::ECS::createEntity());
        return 1;
    }


    int markForRemove(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        unsigned int entity = lua_tointeger(L, 1);
        Core::ECS::markForRemove(entity);
        return 0;
    }

    int addComponent(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");

        unsigned int entityID = (unsigned int)lua_tointeger(L, 1);
        Core::ECS::ComponentType componentType = (Core::ECS::ComponentType)lua_tointeger(L, 2);
          
        lua_pushlightuserdata(L, Core::ECS::addComponent(entityID, componentType));
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
    int setModel(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::MODEL_RENDERER, "Invalid component type");

        const char* modelName = lua_tostring(L, 2);
        EN_ASSERT(modelName != nullptr, "modelName is null");

        Core::ECS::ModelRendererComponent* pModelRenderer = (Core::ECS::ModelRendererComponent*)header;
        pModelRenderer->pModel = Core::Resource::getModel(modelName);
        return 0;
    }
    int setRigidBody(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 8, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::RIGID_BODY, "Invalid component type");

        Core::ECS::RigidBodyComponent* pRigidBody = (Core::ECS::RigidBodyComponent*)header;
        pRigidBody->velocity.x = (float)lua_tonumber(L, 2);
        pRigidBody->velocity.y = (float)lua_tonumber(L, 3);
        pRigidBody->velocity.z = (float)lua_tonumber(L, 4);

        pRigidBody->force.x = (float)lua_tonumber(L, 5);
        pRigidBody->force.y = (float)lua_tonumber(L, 6);
        pRigidBody->force.z = (float)lua_tonumber(L, 7);

        pRigidBody->mass = (float)lua_tonumber(L, 8);

        return 0;
    }
    int getVelocity(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::RIGID_BODY, "Invalid component type");
        Core::ECS::RigidBodyComponent* pRigidBody = (Core::ECS::RigidBodyComponent*)header;
        lua_pushnumber(L, pRigidBody->velocity.x);
        lua_pushnumber(L, pRigidBody->velocity.y);
        lua_pushnumber(L, pRigidBody->velocity.z);

        return 3;
    }
    int setCollisionShapeSphere(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::RIGID_BODY, "Invalid component type");
        Core::ECS::RigidBodyComponent* pRigidBody = (Core::ECS::RigidBodyComponent*)header;
        pRigidBody->colliderType = Core::ECS::ColliderType::SPHERE;
        ((Core::ECS::SphereCollider*)pRigidBody->colliderData)->radius = (float)lua_tonumber(L, 2);

        return 0;
    }
    int setScript(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::SCRIPT, "Invalid component type");

        const char* scriptPath = lua_tostring(L, 2);
        EN_ASSERT(scriptPath != nullptr, "scriptPath is null");

        Core::Lua::loadFile(((Core::ECS::ScriptComponent*)header)->L, scriptPath);
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

