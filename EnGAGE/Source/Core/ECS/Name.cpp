#include "pch.hpp"
#include "Name.hpp"

void Core::Name::OnImGui(ComponentHeader* pHeader)
{
	Component* pName = (Component*)pHeader;
	ImGui::InputText("Name", (char*)pName->name, ECS::MAX_NAME_SIZE);
}

void Core::Name::OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity)
{

}

void Core::Name::Init(ComponentHeader* pHeader)
{
	Name::Component* nameComponent = (Name::Component*)pHeader;
	nameComponent->header.OnImGui = OnImGui;
	nameComponent->header.OnSeralize = OnSeralize;
}
