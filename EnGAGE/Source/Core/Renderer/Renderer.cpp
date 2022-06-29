#include "pch.hpp"
#include "Renderer.hpp"

#include "Core/Window.hpp"
#include "Core/Math.hpp"
#include "AmbientRenderer.hpp"
#include "DirectionalRenderer.hpp"
#include "PointRenderer.hpp"
#include "EntityRenderer.hpp"
#include "MapRenderer.hpp"

#include <glad/glad.h>


namespace Core::Renderer
{
	static AmbientRenderer* gAmbientRenderer;
	static DirectionalRenderer* gDirectionalRenderer;
	static PointRenderer* gPointRenderer;
	static EntityRenderer* gEntityRenderer;
	static MapRenderer* gMapRenderer;
	static GBuffer* gBuffer;

	static bool gRenderCullingSphere = false;
	static F32 gRenderScale = 1.0f;
	static UInt32 gDirectionalShadowMapSize = 2048;
	static F32 gDirectionalShadowDistance = 50.0f;
	static F32 gDirectionalShadowFadeStart = 45.0f;

	void init(UInt32 currentWidth, UInt32 currentHeight)
	{
		gAmbientRenderer = new AmbientRenderer();
		gDirectionalRenderer = new DirectionalRenderer(gDirectionalShadowMapSize, gRenderScale);
		gPointRenderer = new PointRenderer();
		gEntityRenderer = new EntityRenderer();
		gMapRenderer = new MapRenderer();
		gBuffer = new GBuffer();
		gBuffer->update(currentWidth, currentHeight, gRenderScale);
	}
	void onMessage(const Message* pMessage)
	{
		if (auto windowResized = Messenger::messageCast<MessageType::WINDOW_RESIZED, WindowResizedMessage>(pMessage))
		{
			gBuffer->update(windowResized->width, windowResized->height, gRenderScale);
		}
	}

	void shutdown()
	{
		delete gAmbientRenderer;
		delete gDirectionalRenderer;
		delete gPointRenderer;
		delete gEntityRenderer;
		delete gMapRenderer;
		delete gBuffer;
	}


	void render(const Camera& camera)
	{

		auto width = Window::getWidth();
		auto height = Window::getHeight();
		gBuffer->bind(width, height, gRenderScale);
		gEntityRenderer->render(*gBuffer, camera, gRenderCullingSphere);
		gMapRenderer->render(camera);
		gBuffer->unBind(width, height);

		
		gAmbientRenderer->render(*gBuffer);
		gDirectionalRenderer->render(width, height, *gBuffer, camera, *gMapRenderer, gDirectionalShadowDistance, gDirectionalShadowFadeStart);
		gPointRenderer->render(*gBuffer, camera);

		gBuffer->unBindQuad();

	}

	bool& isRenderCullingSphere()
	{
		return gRenderCullingSphere;
	}

	void setRenderScale(F32 scale)
	{
		gRenderScale = scale;
		if (gRenderScale < 0.0f) gRenderScale = 0.0f;
		else if (gRenderScale > 2.0f) gRenderScale = 2.0f;
		gBuffer->update(Window::getWidth(), Window::getHeight(), gRenderScale);
		gDirectionalRenderer->resize(gDirectionalShadowMapSize, gRenderScale);
	}

	void setDirectionalShadowMapSize(UInt32 size)
	{
		gDirectionalShadowMapSize = size;
		gDirectionalRenderer->resize(gDirectionalShadowMapSize, gRenderScale);
	}

	void setDirectionalShadowDistance(F32 value)
	{
		gDirectionalShadowDistance = value;
	}

	void setDirectionalShadowFadeStart(F32 value)
	{
		gDirectionalShadowFadeStart = value;
	}

	GBuffer& getGBuffer()
	{
		return *gBuffer;
	}

}






