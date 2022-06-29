#include "pch.hpp"
#include "Script.hpp"

#include "Core/Scripting.hpp"
#include "Core/Messenger/Messenger.hpp"
#include "Core/LuaHostFunctions.hpp"

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

int Core::Script::luaSetScript(lua_State* L)
{
	CHECK_NUM_ARGS(L, 2);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TSTRING);
	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::SCRIPT, "Invalid component type");

	const char* scriptPath = lua_tostring(L, 2);
	EN_ASSERT(scriptPath != nullptr, "scriptPath is null");

	Script::Component* sciptComponent = (Script::Component*)header;
	Scripting::loadFile(sciptComponent->L, scriptPath);
	strcpy((char*)sciptComponent->scriptPath, scriptPath);
	return 0;
}
