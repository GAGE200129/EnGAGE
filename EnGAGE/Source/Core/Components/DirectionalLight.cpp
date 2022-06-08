#include "pch.hpp"
#include "DirectionalLight.hpp"

void Core::DirectionalLight::OnImGui(ComponentHeader* pHeader)
{
	Component* pLight = (Component*)pHeader;
	ImGui::DragFloat3("Direction", &pLight->direction.x, 0.01f, -1, 1);
	ImGui::ColorEdit3("Color", &pLight->color.x);
	ImGui::DragFloat("Intensity", &pLight->intensity, 0.01f, 0.0f, 1.0f);
}

void Core::DirectionalLight::OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity)
{
	DirectionalLight::Component* component = (DirectionalLight::Component*)pHeader;

	out << "_setDirectionalLight(_addComponent(" << entity << ", " << getHint().name << "), " <<
		component->direction.x << ", " << component->direction.y << ", " << component->direction.z << ", " <<
		component->color.x << ", " << component->color.y << ", " << component->color.z << ", " <<
		component->intensity << ")\n";
}

void Core::DirectionalLight::Init(ComponentHeader* pHeader)
{
	Component* component = (Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->direction.x = 0.0f;
	component->direction.y = -1.0f;
	component->direction.z = 0.0f;
	component->color.x = 1.0f;
	component->color.y = 1.0f;
	component->color.z = 1.0f;
	component->intensity = 1.0f;
}
