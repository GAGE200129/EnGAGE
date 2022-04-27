#include "pch.hpp"

#include "Components.hpp"
#include "Messenger.hpp"


namespace Core
{

	template<ComponentType> ComponentData ComponentDataEnum = { "INVALID", 0 };
	template<> ComponentData ComponentDataEnum<ComponentType::NAME> = { "NAME", sizeof(NameComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::TRANSFORM> = { "TRANSFORM", sizeof(TransformComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::MODEL_RENDERER> = { "MODEL_RENDERER", sizeof(ModelRendererComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::SCRIPT> = { "SCRIPT", sizeof(ScriptComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::RIGID_BODY> = { "RIGID_BODY", sizeof(RigidBodyComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::DIRECTIONAL_LIGHT> = { "DIRECTIONAL_LIGHT", sizeof(DirectionalLightComponent) };

	const ComponentData& getComponentData(ComponentType type)
	{

		switch (type)
		{
		case ComponentType::NAME:
			return ComponentDataEnum<ComponentType::NAME>;
		case ComponentType::TRANSFORM:
			return ComponentDataEnum<ComponentType::TRANSFORM>;
		case ComponentType::MODEL_RENDERER:
			return ComponentDataEnum<ComponentType::MODEL_RENDERER>;
		case ComponentType::SCRIPT:
			return ComponentDataEnum<ComponentType::SCRIPT>;
		case ComponentType::RIGID_BODY:
			return ComponentDataEnum<ComponentType::RIGID_BODY>;
		case ComponentType::DIRECTIONAL_LIGHT:
			return ComponentDataEnum<ComponentType::DIRECTIONAL_LIGHT>;
		}
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
		return  ComponentDataEnum<ComponentType::COUNT>;
	}
	
	void initComponent(ComponentHeader* pHeader, ComponentType type)
	{
		switch (type)
		{
		case ComponentType::TRANSFORM:
		{
			TransformComponent* transformComponent = (TransformComponent*)pHeader;
			transformComponent->isStatic = true;
			transformComponent->x = 0;
			transformComponent->y = 0;
			transformComponent->z = 0;
			transformComponent->rw = 1;
			transformComponent->rx = 0;
			transformComponent->ry = 0;
			transformComponent->rz = 0;
			transformComponent->sx = 1;
			transformComponent->sy = 1;
			transformComponent->sz = 1;
			break;
		}
		case ComponentType::MODEL_RENDERER:
		{
			ModelRendererComponent* component = (ModelRendererComponent*)pHeader;
			component->pModel = nullptr;
			memset(component->modelPath, 0, MAX_NAME_SIZE);
			break;
		}

		case ComponentType::SCRIPT:
		{
			ScriptComponent* component = (ScriptComponent*)pHeader;

			auto request = Messenger::request(RequestType::NEW_SCRIPT);
			memcpy(&component->L, request.data, sizeof(lua_State*));
			memset(component->scriptPath, 0, MAX_NAME_SIZE);
			break;
		}
		case ComponentType::RIGID_BODY:
		{
			RigidBodyComponent* component = (RigidBodyComponent*)pHeader;

			auto request = Messenger::request(RequestType::NEW_RIGID_BODY);
			memcpy(&component->pRigidbody, request.data, sizeof(btRigidBody*));
			component->collisionShapeType = 0;
			break;
		}
		case ComponentType::DIRECTIONAL_LIGHT:
		{
			DirectionalLightComponent* component = (DirectionalLightComponent*)pHeader;
			component->direction.x = 0.0f;
			component->direction.y = -1.0f;
			component->direction.z = 0.0f;
			component->color.x = 1.0f;
			component->color.y = 1.0f;
			component->color.z = 1.0f;
			component->intensity = 1.0f;

			break;
		}
		}
	}
	void destroyComponent(ComponentHeader* pHeader, ComponentType type)
	{
		switch (type)
		{
		case ComponentType::SCRIPT:
		{
			ScriptComponent* component = (ScriptComponent*)pHeader;
			if (component)
			{
				Message message = {MessageType::REMOVE_SCRIPT};

				memcpy(message.message, &component->L, sizeof(lua_State*));
				Messenger::recieveMessage(&message);
			}
			break;
		}
		}
	}
}