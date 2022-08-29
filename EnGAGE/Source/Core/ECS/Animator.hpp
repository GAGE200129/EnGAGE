#pragma once
#include "ComponentHeader.hpp"
#include "ECSConstants.hpp"
#include "ComponentHint.hpp"

#include "Core/Data/Model.hpp"

namespace Core
{
	namespace Animator
	{
		struct Component
		{
			ComponentHeader header;
			float duration;
			float currentTime;
			AnimationData* pCurrentAnimation;
		};


		void OnImGui(ComponentHeader* pHeader);
		void OnSeralize(ComponentHeader* pComponent, std::ofstream& out, const String& entity);
		void Destroy(ComponentHeader* pHeader);
		void Init(ComponentHeader* pHeader);



		inline static ComponentHint getHint()
		{
			return ComponentHint{ "ANIMATOR", sizeof(Component) };
		}
	}
}