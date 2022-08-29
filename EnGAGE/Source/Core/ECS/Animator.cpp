#include "pch.hpp"
#include "Animator.hpp"

void Core::Animator::OnImGui(ComponentHeader* pHeader)
{
}

void Core::Animator::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
}

void Core::Animator::Destroy(ComponentHeader* pHeader)
{
}

void Core::Animator::Init(ComponentHeader* pHeader)
{
	Animator::Component* component = (Animator::Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->header.Destroy = Destroy;
	component->duration = 0.0f;
	component->currentTime = 0.0f;
	component->pCurrentAnimation = nullptr;
}
