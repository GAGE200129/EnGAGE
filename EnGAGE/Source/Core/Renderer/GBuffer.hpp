#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class GBuffer
	{
	public:
		GBuffer();		
		~GBuffer();
		void update(UInt32 inWidth, UInt32 inHeight, F32 scale);

		void bind(UInt32 inWidth, UInt32 inHeight, F32 scale);

		void unBind(UInt32 inWidth, UInt32 inHeight);


		void bindQuad();
		void renderQuad() const;
		void unBindQuad();
	private:
		UInt32 quadVAO;
		UInt32 quadVBO;
		UInt32 FBO, RBO;
		UInt32 positionTex, normalTex, colorTex;
	};
}