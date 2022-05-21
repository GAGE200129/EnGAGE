#include "pch.hpp"
#include "Renderer.hpp"

#include "Core/Window.hpp"
#include "Core/Math.hpp"
#include "AmbientRenderer.hpp"
#include "DirectionalRenderer.hpp"
#include "PointRenderer.hpp"
#include "EntityRenderer.hpp"
#include "GBuffer.hpp"
#include <glad/glad.h>


namespace Core::Renderer
{
	static AmbientRenderer* gAmbientRenderer;
	static DirectionalRenderer* gDirectionalRenderer;
	static PointRenderer* gPointRenderer;
	static EntityRenderer* gEntityRenderer;
	static GBuffer* gBuffer;

	static bool gRenderCullingSphere = false;
	static F32 gRenderScale = 1.0f;

	void init(UInt32 currentWidth, UInt32 currentHeight)
	{
		gAmbientRenderer = new AmbientRenderer();
		gDirectionalRenderer = new DirectionalRenderer();
		gPointRenderer = new PointRenderer();
		gEntityRenderer = new EntityRenderer();
		gBuffer = new GBuffer();
		gBuffer->update(currentWidth, currentHeight, gRenderScale);
	}
	void onMessage(const Message* pMessage)
	{
		if (auto setScale = Messenger::messageCast<MessageType::RENDERER_SET_SCALE, RendererSetScaleMessage>(pMessage))
		{
			gRenderScale = setScale->scale;
			if (gRenderScale < 0.0f) gRenderScale = 0.0f;
			else if (gRenderScale > 2.0f) gRenderScale = 2.0f;
			gBuffer->update(Window::getWidth(), Window::getHeight(), gRenderScale);
		}
		else if (auto windowResized = Messenger::messageCast<MessageType::WINDOW_RESIZED, WindowResizedMessage>(pMessage))
		{
			gBuffer->update(windowResized->width, windowResized->height, gRenderScale);
		}
		else if (pMessage->type == MessageType::RENDERER_TOGGLE_CULLING_SPHERE)
		{
			gRenderCullingSphere = !gRenderCullingSphere;
		}
	}

	void shutdown()
	{
		delete gAmbientRenderer;
		delete gDirectionalRenderer;
		delete gPointRenderer;
		delete gEntityRenderer;
		delete gBuffer;
	}


	void render(const Camera& camera)
	{
		gBuffer->bind(Window::getWidth(), Window::getHeight(), gRenderScale);
		gBuffer->bindShader();
		gBuffer->uploadProjView(Math::calculateProjectionView(camera));
		gEntityRenderer->render(*gBuffer, camera, gRenderCullingSphere);
		gBuffer->unBind(Window::getWidth(), Window::getHeight());

		gBuffer->bindQuad();
		gAmbientRenderer->render(*gBuffer);
		gDirectionalRenderer->render(*gBuffer, camera);
		gPointRenderer->render(*gBuffer, camera);
		gBuffer->unBindQuad();

	}

}






