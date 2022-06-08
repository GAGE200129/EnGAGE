#include "pch.hpp"
#include "Script.hpp"

#include "Core/Scripting.hpp"
#include "Core/Messenger/Messenger.hpp"

void Core::Script::OnImGui(ComponentHeader* pHeader)
{
	Component* pScript = (Component*)pHeader;

	ImGui::Text("Script: %p", pScript->L);

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_SCRIPT"))
		{
			Core::Scripting::loadFile(pScript->L, String((char*)payload->Data));
			memcpy(pScript->scriptPath, (char*)payload->Data, payload->DataSize);
		}
		ImGui::EndDragDropTarget();
	}
}

void Core::Script::OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity)
{
	Script::Component* component = (Script::Component*)pComponent;
	out << "_setScript(" << "_addComponent(" << entity << ", " << getHint().name << ")" << ", \"" << component->scriptPath << "\")\n";
}

void Core::Script::Destroy(ComponentHeader* pHeader)
{
	Script::Component* component = (Script::Component*)pHeader;
	if (component)
	{
		Scripting::removeScript(component->L);
	}
}

void Core::Script::Init(ComponentHeader* pHeader)
{
	Component* component = (Component*)pHeader;
	component->header.OnImGui = OnImGui;
	component->header.OnSeralize = OnSeralize;
	component->header.Destroy = Destroy;
	component->L = Scripting::newScript();
	memset(component->scriptPath, 0, ECS::MAX_NAME_SIZE);
}
