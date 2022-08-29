#include "pch.hpp"
#include "Components.hpp"
#include "Core/Scripting.hpp"
#include "Core/Physics/Physics.hpp"
#include "ECS.hpp"
#include "Core/Resource.hpp"

#include "Name.hpp"
#include "RigidBody.hpp"
#include "KinematicBody.hpp"
#include "Transform.hpp"
#include "ModelRenderer.hpp"
#include "Script.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "Animator.hpp"

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
			TO_DATA(KINEMATIC_BODY, KinematicBody);
			TO_DATA(DIRECTIONAL_LIGHT, DirectionalLight);
			TO_DATA(POINT_LIGHT, PointLight);
			TO_DATA(ANIMATOR, Animator);
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
			TO_INIT(KINEMATIC_BODY, KinematicBody);
			TO_INIT(DIRECTIONAL_LIGHT, DirectionalLight);
			TO_INIT(POINT_LIGHT, PointLight);
			TO_INIT(ANIMATOR, Animator);
		default:
			EN_ASSERT(false, "Unknown component: {}", (unsigned int)type);
			break;
		}	
	}
}