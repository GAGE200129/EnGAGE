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

	void init(UInt32 currentWidth, UInt32 currentHeight)
	{
		gAmbientRenderer = new AmbientRenderer();
		gDirectionalRenderer = new DirectionalRenderer();
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
		gMapRenderer->render(*gBuffer, camera);
		gBuffer->unBind(width, height);

		//gBuffer->getPost().bind(width, height, gRenderScale);
		gBuffer->bindQuad();
		gAmbientRenderer->render(*gBuffer);
		gDirectionalRenderer->render(*gBuffer, camera);
		gPointRenderer->render(*gBuffer, camera);
		gBuffer->unBindQuad();
		//gBuffer->getPost().unbind(width, height);

		//glBindVertexArray(gBuffer->getQuadVAO());
		//
		//glBindVertexArray(0);

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
	}

	GBuffer& getGBuffer()
	{
		return *gBuffer;
	}

}






