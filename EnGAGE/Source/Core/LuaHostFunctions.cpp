#include "pch.hpp"
#include "LuaHostFunctions.hpp"

#include "ECS.hpp"
#include "InputCodes.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Resource.hpp"
#include "Script.hpp"
#include "Scene.hpp"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace LuaHostFunctions
{
    //ECS
    int createEntity(lua_State* L);
    int markForRemove(lua_State* L);
    int addComponent(lua_State* L);
    int getComponent(lua_State* L);


    //Input
    int keyPressed(lua_State* L);
    int keyPressedOnce(lua_State* L);
    int buttonPressed(lua_State* L);
    int getCursorPosDelta(lua_State* L);
    int toggleCursor(lua_State* L);
    int isCursorLocked(lua_State* L);

    //Transform
    int setPosition(lua_State* L);
    int setRotation(lua_State* L);
    int setScale(lua_State* L);
    int translate(lua_State* L);
    int rotate(lua_State* L);
    int scale(lua_State* L);

    //Modelrenderer
    int setModel(lua_State* L);

    //Ridgidbody
    int setRigidBody(lua_State* L);
    int setCollisionShapeSphere(lua_State* L);
    int setCollisionShapePlane(lua_State* L);

    //Script
    int setScript(lua_State* L);

    //Render
    int updateCamera(lua_State* L);

    //Scene
    int loadScene(lua_State* L);
    int saveScene(lua_State* L);

    void registerAllFunctions(lua_State* L)
    {
      
    }
    void registerAllScriptFunctions(lua_State* L)
    {
        lua_register(L, "_markForRemove", markForRemove);
        lua_register(L, "_getComponent", getComponent);
        lua_register(L, "_keyPressed", keyPressed);
        lua_register(L, "_keyPressedOnce", keyPressedOnce);
        lua_register(L, "_buttonPressed", buttonPressed);
        lua_register(L, "_getCursorPosDelta", getCursorPosDelta);
        lua_register(L, "_toggleCursor", toggleCursor);
        lua_register(L, "_isCursorLocked", isCursorLocked);
        lua_register(L, "_setPosition", setPosition);
        lua_register(L, "_setRotation", setRotation);
        lua_register(L, "_setScale", setScale);
        lua_register(L, "_translate", translate);
        lua_register(L, "_rotate", rotate);
        lua_register(L, "_scale", scale);
        lua_register(L, "_updateCamera", updateCamera);
    }
    void registerAllSceneFunctions(lua_State* L)
    {
        lua_register(L, "_createEntity", createEntity);
        lua_register(L, "_markForRemove", markForRemove);
        lua_register(L, "_addComponent", addComponent);
        lua_register(L, "_getComponent", getComponent);
        lua_register(L, "_setModel", setModel);
        lua_register(L, "_setRigidBody", setRigidBody);
        lua_register(L, "_setPosition", setPosition);
        lua_register(L, "_setRotation", setRotation);
        lua_register(L, "_setScale", setScale);
        lua_register(L, "_setCollisionShapeSphere", setCollisionShapeSphere);
        lua_register(L, "_setCollisionShapePlane", setCollisionShapePlane);
        lua_register(L, "_setScript", setScript);
    }
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
        strcpy(pModelRenderer->modelPath, modelName);
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
    int setCollisionShapePlane(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 5, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::RIGID_BODY, "Invalid component type");
        Core::ECS::RigidBodyComponent* pRigidBody = (Core::ECS::RigidBodyComponent*)header;
        pRigidBody->colliderType = Core::ECS::ColliderType::PLANE;
        ((Core::ECS::PlaneCollider*)pRigidBody->colliderData)->x = (float)lua_tonumber(L, 2);
        ((Core::ECS::PlaneCollider*)pRigidBody->colliderData)->y = (float)lua_tonumber(L, 3);
        ((Core::ECS::PlaneCollider*)pRigidBody->colliderData)->z = (float)lua_tonumber(L, 4);
        ((Core::ECS::PlaneCollider*)pRigidBody->colliderData)->distance = (float)lua_tonumber(L, 5);

        return 0;
    }
    int setScript(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 2, "Invalid argument");
        Core::ECS::ComponentHeader* header = (Core::ECS::ComponentHeader*)lua_touserdata(L, 1);
        EN_ASSERT(header->type == Core::ECS::ComponentType::SCRIPT, "Invalid component type");

        const char* scriptPath = lua_tostring(L, 2);
        EN_ASSERT(scriptPath != nullptr, "scriptPath is null");

        Core::ECS::ScriptComponent* sciptComponent = (Core::ECS::ScriptComponent*)header;
        Core::Script::loadFile(sciptComponent->L, scriptPath);
        strcpy(sciptComponent->scriptPath, scriptPath);
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
    int loadScene(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        const char* scenePath = lua_tostring(L, 1);
        EN_ASSERT(scenePath != nullptr, "scenePath is null");

        Core::Scene::loadScene(String(scenePath));

        return 0;
    }
    int saveScene(lua_State* L)
    {
        EN_ASSERT(lua_gettop(L) == 1, "Invalid argument");
        const char* scenePath = lua_tostring(L, 1);
        EN_ASSERT(scenePath != nullptr, "scenePath is null");

        Core::Scene::saveScene(String(scenePath));

        return 0;
    }
}

