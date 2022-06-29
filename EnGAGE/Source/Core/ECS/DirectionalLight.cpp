#include "pch.hpp"
#include "DirectionalLight.hpp"

#include "Core/LuaHostFunctions.hpp"

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

int Core::DirectionalLight::luaSetDirectionalLight(lua_State* L)
{
	CHECK_NUM_ARGS(L, 8);
	CHECK_ARG(L, 1, LUA_TLIGHTUSERDATA);
	CHECK_ARG(L, 2, LUA_TNUMBER);
	CHECK_ARG(L, 3, LUA_TNUMBER);
	CHECK_ARG(L, 4, LUA_TNUMBER);
	CHECK_ARG(L, 5, LUA_TNUMBER);
	CHECK_ARG(L, 6, LUA_TNUMBER);
	CHECK_ARG(L, 7, LUA_TNUMBER);
	CHECK_ARG(L, 8, LUA_TNUMBER);

	ComponentHeader* header = (ComponentHeader*)lua_touserdata(L, 1);
	EN_ASSERT(header->type == ComponentType::DIRECTIONAL_LIGHT, "Invalid component type");
	DirectionalLight::Component* component = (DirectionalLight::Component*)header;
	component->direction.x = (float)lua_tonumber(L, 2);
	component->direction.y = (float)lua_tonumber(L, 3);
	component->direction.z = (float)lua_tonumber(L, 4);

	component->color.x = (float)lua_tonumber(L, 5);
	component->color.y = (float)lua_tonumber(L, 6);
	component->color.z = (float)lua_tonumber(L, 7);

	component->intensity = (float)lua_tonumber(L, 8);

	return 0;
}
