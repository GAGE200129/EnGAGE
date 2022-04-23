#include "pch.hpp"

#include "Components.hpp"
#include "Script.hpp"

namespace Core::ECS
{
	template<ComponentType> ComponentData ComponentDataEnum = { "INVALID", 0 };
	template<> ComponentData ComponentDataEnum<ComponentType::NAME> = { "NAME", sizeof(NameComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::TRANSFORM> = { "TRANSFORM", sizeof(TransformComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::MODEL_RENDERER> = { "MODEL_RENDERER", sizeof(ModelRendererComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::SCRIPT> = { "SCRIPT", sizeof(ScriptComponent) };
	template<> ComponentData ComponentDataEnum<ComponentType::RIGID_BODY> = { "RIGID_BODY", sizeof(RigidBodyComponent) };

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
			component->L = Script::newScript();
			memset(component->scriptPath, 0, MAX_NAME_SIZE);
			break;
		}
		case ComponentType::RIGID_BODY:
		{
			RigidBodyComponent* component = (RigidBodyComponent*)pHeader;
			component->mass = 0.0f;
			component->force = { 0, 0, 0 };
			component->velocity = { 0, 0, 0 };
			component->colliderType = 0;
			memset(component->colliderData, 0, MAX_COLLIDER_BUFFER_SIZE);

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
				Script::removeScript(component->L);
			break;
		}
		}
	}
}