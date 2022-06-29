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

int Core::PointLight::luaSetPointLight(lua_State* L)
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
	EN_ASSERT(header->type == ComponentType::POINT_LIGHT, "Invalid component type");
	PointLight::Component* component = (PointLight::Component*)header;
	component->color.x = (float)lua_tonumber(L, 2);
	component->color.y = (float)lua_tonumber(L, 3);
	component->color.z = (float)lua_tonumber(L, 4);
	component->intensity = (float)lua_tonumber(L, 5);

	component->constant = (float)lua_tonumber(L, 5);
	component->linear = (float)lua_tonumber(L, 6);
	component->exponent = (float)lua_tonumber(L, 7);


	return 0;
}
