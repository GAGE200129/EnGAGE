#include "pch.hpp"
#include "PointLight.hpp"

void Core::PointLight::OnImGui(ComponentHeader* pHeader)
{
	Component* pLight = (Component*)pHeader;
	ImGui::DragFloat3("Color", &pLight->color.x, 0.1f, 0, 1);
	ImGui::DragFloat("Intensity", &pLight->intensity, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Constant", &pLight->constant, 0.01f);
	ImGui::DragFloat("Linear", &pLight->linear, 0.001f);
	ImGui::DragFloat("Exponent", &pLight->exponent, 0.0001f);
}

void Core::PointLight::OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity)
{
	PointLight::Component* component = (PointLight::Component*)pHeader;

	out << "_setPointLight(_addComponent(" << entity << ", " << getHint().name << "), " <<
		component->color.x << ", " << component->color.y << ", " << component->color.z << ", " <<
		component->intensity << ", " <<
		component->constant << ", " << component->linear << ", " << component->exponent << ")\n";
}

void Core::PointLight::Init(ComponentHeader* pHeader)
{
	Component* component = (Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->color.x = 1.0f;
	component->color.y = 1.0f;
	component->color.z = 1.0f;
	component->intensity = 1.0f;

	component->constant = 1.0f;
	component->linear = 0.1f;
	component->exponent = 0.01f;
}
