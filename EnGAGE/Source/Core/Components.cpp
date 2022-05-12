#include "pch.hpp"
#include "Components.hpp"
#include "Scripting.hpp"
#include "Physics.hpp"
#include "ECS.hpp"
#include "Physics.hpp"
#include "Resource.hpp"

#include "Messenger.hpp"
#include "Components/Name.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/Script.hpp"
#include "Components/DirectionalLight.hpp"
#include "Components/PointLight.hpp"

#define TO_DATA(type, fnName) case ComponentType::type: return fnName::getHint()


#define TO_INIT(type, fnName)\
		case ComponentType::type:\
		{\
		fnName::Init(pHeader);\
		break;\
		}

namespace Core
{
	const ComponentHint getComponentHint(ComponentType type)
	{

		switch (type)
		{
			TO_DATA(NAME, Name);
			TO_DATA(TRANSFORM, Transform);
			TO_DATA(MODEL_RENDERER, ModelRenderer);
			TO_DATA(SCRIPT, Script);
			TO_DATA(RIGID_BODY, RigidBody);
			TO_DATA(DIRECTIONAL_LIGHT, DirectionalLight);
			TO_DATA(POINT_LIGHT, PointLight);
		}
		EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
		return  {"INVALID", 0};
	}
	
	void initComponent(ComponentHeader* pHeader, ComponentType type)
	{
		switch (type)
		{
			TO_INIT(NAME, Name);
			TO_INIT(TRANSFORM, Transform);
			TO_INIT(MODEL_RENDERER, ModelRenderer);
			TO_INIT(SCRIPT, Script);
			TO_INIT(RIGID_BODY, RigidBody);
			TO_INIT(DIRECTIONAL_LIGHT, DirectionalLight);
			TO_INIT(POINT_LIGHT, PointLight);
		default:
			EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
			break;
		}

		
	}

	
}