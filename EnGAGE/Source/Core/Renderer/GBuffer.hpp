#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class PostProcess
	{
	public:
		PostProcess();
		~PostProcess();

		void update(UInt32 inWidth, UInt32 inHeight, F32 scale);
		void bind(UInt32 inWidth, UInt32 inHeight, F32 scale) const;
		void unbind(UInt32 inWidth, UInt32 inHeight) const;

		inline UInt32 getColorTex() const { return colorTex; }
	private:
		UInt32 fbo;
		UInt32 colorTex;
	};
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

		inline const PostProcess& getPost() const { return post; }
		inline UInt32 getQuadVAO() const { return quadVAO; };
	private:
		PostProcess post;
		UInt32 quadVAO;
		UInt32 quadVBO;
		UInt32 FBO, RBO;
		UInt32 positionTex, normalTex, colorTex, depthTex;
	};
}