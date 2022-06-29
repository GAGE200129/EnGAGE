#include "pch.hpp"
#include "ModelRenderer.hpp"

#include "Core/Resource.hpp"
#include "Core/LuaHostFunctions.hpp"

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

int Core::ModelRenderer::luaSetModel(lua_State* L)
{
	CHECK_NUM_ARGS(L, 2);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TSTRING);


	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::MODEL_RENDERER, "Invalid component type");

	const char* modelName = lua_tostring(L, 2);
	EN_ASSERT(modelName != nullptr, "modelName is null");

	ModelRenderer::Component* pModelRenderer = (ModelRenderer::Component*)header;
	pModelRenderer->pModel = Resource::getModel(modelName);
	strcpy((char*)pModelRenderer->modelPath, modelName);
	return 0;
}