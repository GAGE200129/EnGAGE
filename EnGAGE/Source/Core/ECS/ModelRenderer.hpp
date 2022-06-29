#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"
namespace Core
{
	struct Model;
	namespace ModelRenderer
	{
		struct Component
		{
			ComponentHeader header;
			Byte modelPath[ECS::MAX_NAME_SIZE];
			const Model* pModel;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pHeader, std::ofstream& out, const String& entity);
		void Init(ComponentHeader* pHeader);

		int luaSetModel(lua_State* L);

		inline static ComponentHint getHint()
		{
			return ComponentHint{ "MODEL_RENDERER", sizeof(Component) };
		}
	}
}