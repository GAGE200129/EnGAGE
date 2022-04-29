#include "pch.hpp"

#include "Components.hpp"
#include "Messenger.hpp"

#define TO_SWITCH(x) case ComponentType::x: return ComponentDataEnum<ComponentType::x>

namespace Core
{

	template<ComponentType> ComponentData ComponentDataEnum = { "INVALID", 0 };
	template<> ComponentData ComponentDataEnum<ComponentType::NAME> = { "NAME", sizeof(NameComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::TRANSFORM> = { "TRANSFORM", sizeof(TransformComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::MODEL_RENDERER> = { "MODEL_RENDERER", sizeof(ModelRendererComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::SCRIPT> = { "SCRIPT", sizeof(ScriptComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::RIGID_BODY> = { "RIGID_BODY", sizeof(RigidBodyComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::DIRECTIONAL_LIGHT> = { "DIRECTIONAL_LIGHT", sizeof(DirectionalLightComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::POINT_LIGHT> = { "POINT_LIGHT", sizeof(PointLightComponent) };

	const ComponentData& getComponentData(ComponentType type)
	{

		switch (type)
		{
			TO_SWITCH(NAME);
			TO_SWITCH(TRANSFORM);
			TO_SWITCH(MODEL_RENDERER);
			TO_SWITCH(SCRIPT);
			TO_SWITCH(RIGID_BODY);
			TO_SWITCH(DIRECTIONAL_LIGHT);
			TO_SWITCH(POINT_LIGHT);
		}
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
		return  ComponentDataEnum<ComponentType::COUNT>;
	}
	
	void initComponent(unsigned int entityID, ComponentHeader* pHeader, ComponentType type)
	{
		switch (type)
		{
		case ComponentType::TRANSFORM:
		{
			TransformComponent* transformComponent = (TransformComponent*)pHeader;
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
			auto request = Messenger::request(RequestType::NEW_RIGID_BODY, sizeof(unsigned int), &entityID);
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
		case ComponentType::POINT_LIGHT:
		{
			PointLightComponent* component = (PointLightComponent*)pHeader;
			component->color.x = 1.0f;
			component->color.y = 1.0f;
			component->color.z = 1.0f;
			component->intensity = 1.0f;

			component->constant = 1.0f;
			component->linear = 0.1f;
			component->exponent = 0.01f;

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
				RemoveScriptMessage	message;
				message.L = component->L;
				Messenger::recieveMessage(MessageType::REMOVE_SCRIPT, &message);
			}
			break;
		}

		case ComponentType::RIGID_BODY:
		{
			RigidBodyComponent* component = (RigidBodyComponent*)pHeader;
			if (component)
			{
				RemoveRigidBodyMessage message;
				message.body = component->pRigidbody;
				Messenger::recieveMessage(MessageType::REMOVE_RIGID_BODY, &message);
			}
			break;
		}
		}
	}
}