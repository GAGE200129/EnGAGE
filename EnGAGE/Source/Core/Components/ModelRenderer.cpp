#include "pch.hpp"
#include "ModelRenderer.hpp"

#include "Core/Resource.hpp"

void Core::ModelRenderer::OnImGui(ComponentHeader* pHeader)
{
	Component* pModel = (Component*)pHeader;

	ImGui::Text("Model: %p", pModel->pModel);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_MODEL"))
		{
			pModel->pModel = Core::Resource::getModel((const char*)payload->Data);
			strcpy((char*)pModel->modelPath, (const char*)payload->Data);
		}
		ImGui::EndDragDropTarget();
	}
}

void Core::ModelRenderer::OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity)
{
	ModelRenderer::Component* component = (ModelRenderer::Component*)pHeader;
	out << "_setModel(" << "_addComponent(" << entity << ", " << getHint().name << ")" << ", \"" << component->modelPath << "\")\n";
}

void Core::ModelRenderer::Init(ComponentHeader* pHeader)
{
	Component* component = (Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->pModel = nullptr;
	memset(component->modelPath, 0, ECS::MAX_NAME_SIZE);
}
