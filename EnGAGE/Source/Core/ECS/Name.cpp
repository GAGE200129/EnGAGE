#include "pch.hpp"
#include "Name.hpp"

#include "Core/LuaHostFunctions.hpp"

void Core::Name::OnImGui(ComponentHeader* pHeader)
{
	Component* pName = (Component*)pHeader;
	ImGui::InputText("Name", (char*)pName->name, ECS::MAX_NAME_SIZE);
}

void Core::Name::OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity)
{
	Name::Component* name = (Name::Component*)pHeader;
	out << "_setName(_addComponent(" << entity << ", " << getHint().name << "), \"" << name->name << "\")\n";
}

void Core::Name::Init(ComponentHeader* pHeader)
{
	Name::Component* nameComponent = (Name::Component*)pHeader;
	nameComponent->header.OnImGui = OnImGui;
	nameComponent->header.OnSeralize = OnSeralize;
}

int Core::Name::luaSetName(lua_State* L)
{
	CHECK_NUM_ARGS(L, 2);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TSTRING);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::NAME, "Invalid component type");
	Name::Component* name = (Name::Component*)header;
	const char* luaName = lua_tostring(L, 2);
	EN_ASSERT(strlen(luaName) != 0, "str is empty");
	strcpy(name->name, luaName);

	return 0;
}
