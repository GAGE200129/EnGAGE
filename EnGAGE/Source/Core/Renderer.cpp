#include "pch.hpp"
#include "Renderer.hpp"

#include "ECS.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"
#include "DebugRenderer.hpp"
#include "Math.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/DirectionalLight.hpp"
#include "Components/PointLight.hpp"
#include "Renderer/AmbientRenderer.hpp"
#include "Renderer/DirectionalRenderer.hpp"
#include "Renderer/PointRenderer.hpp"
#include <glad/glad.h>


namespace Core::Renderer
{
	class GBufferShader : public Shader
	{
	public:
		GBufferShader()
		{
			loadVertexShader("Resources/Shaders/GBuffer_VS.glsl");
			loadFragmentShader("Resources/Shaders/GBuffer_FS.glsl");
			compile();

			mProjViewLoc = registerUniform("uProjView");
			mModelLoc = registerUniform("uModel");
		}
		~GBufferShader()
		{
			cleanup();
		}

		void uploadProjView(const Mat4x4& mat)
		{
			uploadMat4x4(mProjViewLoc, mat);
		}
		void uploadModel(const Mat4x4& mat)
		{
			uploadMat4x4(mModelLoc, mat);
		}
	private:
		Int32 mProjViewLoc, mModelLoc;
	};
	struct MeshData
	{
		UInt32 vao;
		UInt64 vertexCount;
		UInt32 eboDataType;
		Mat4x4 worldTransform;
		bool cull;
		float boundingSphereRadius;
	};

	void initQuad();;
	void updateGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale);

	static UInt32 gQuadVAO;
	static UInt32 gQuadVBO;
	static DynArr<MeshData> gMeshData;

	static GBufferShader* gBufferShader;
	static AmbientRenderer* gAmbientRenderer;
	static DirectionalRenderer* gDirectionalRenderer;
	static PointRenderer* gPointRenderer;

	static UInt32 gFBO, gRBO;
	static UInt32 gPoisitonTex, gNormalTex, gColorTex;

	static bool gRenderCullingSphere = false;
	static F32 gRenderScale = 1.0f;

	void init(UInt32 currentWidth, UInt32 currentHeight)
	{
		gBufferShader = new GBufferShader();
		gAmbientRenderer = new AmbientRenderer();
		gDirectionalRenderer = new DirectionalRenderer();
		gPointRenderer = new PointRenderer();
		glGenFramebuffers(1, &gFBO);
		glGenRenderbuffers(1, &gRBO);
		glGenTextures(1, &gPoisitonTex);
		glGenTextures(1, &gNormalTex);
		glGenTextures(1, &gColorTex);
		updateGBuffer(currentWidth, currentHeight, gRenderScale);
		initQuad();
	}
	void onMessage(const Message* pMessage)
	{
		if (auto setScale = Messenger::messageCast<MessageType::RENDERER_SET_SCALE, RendererSetScaleMessage>(pMessage))
		{
			gRenderScale = setScale->scale;
			if (gRenderScale < 0.0f) gRenderScale = 0.0f;
			else if (gRenderScale > 2.0f) gRenderScale = 2.0f;
			updateGBuffer(Window::getWidth(), Window::getHeight(), gRenderScale);
		}
		else if (auto windowResized = Messenger::messageCast<MessageType::WINDOW_RESIZED, WindowResizedMessage>(pMessage))
		{
			updateGBuffer(windowResized->width, windowResized->height, gRenderScale);
		}
		else if (pMessage->type == MessageType::RENDERER_TOGGLE_CULLING_SPHERE)
		{
			gRenderCullingSphere = !gRenderCullingSphere;
		}
	}

	void shutdown()
	{
		delete gBufferShader;
		delete gAmbientRenderer;
		delete gDirectionalRenderer;
		delete gPointRenderer;
		glDeleteFramebuffers(1, &gFBO);
		glDeleteRenderbuffers(1, &gRBO);
		glDeleteTextures(1, &gPoisitonTex);
		glDeleteTextures(1, &gNormalTex);
		glDeleteTextures(1, &gColorTex);
		glDeleteVertexArrays(1, &gQuadVAO);
		glDeleteBuffers(1, &gQuadVBO);
	}

	void submitMesh(UInt32 vao, UInt64 vertexCount, UInt32 eboDataType, const Mat4x4& worldTransform, bool cull, float sphereRadius)
	{
		gMeshData.push_back({ vao, vertexCount, eboDataType, worldTransform, cull, sphereRadius });
	}

	void render(const Camera& camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Window::getWidth() * gRenderScale, Window::getHeight() * gRenderScale);

		//Render all to Geometry buffer
		Mat4x4 gProjView = Math::calculateProjectionView(camera);
		gBufferShader->bind();
		gBufferShader->uploadProjView(gProjView);
		Math::Frustum frustum = Math::createFrustum();

		for (const auto& mesh : gMeshData)
		{
			auto isOnFrustum = [](const Math::Frustum& frustum, const glm::vec3& point, const float radius) -> bool
			{
				auto isOnOrForwardPlan = [&point, &radius](const Vec4& plan) -> bool
				{
					const float distanceToPoint = glm::dot(Vec3(plan), point) + plan.w + radius;
					return (distanceToPoint > 0);
				};
				return (isOnOrForwardPlan(frustum.leftFace) &&
					isOnOrForwardPlan(frustum.rightFace) &&
					isOnOrForwardPlan(frustum.farFace) &&
					isOnOrForwardPlan(frustum.nearFace) &&
					isOnOrForwardPlan(frustum.topFace) &&
					isOnOrForwardPlan(frustum.bottomFace));
			};

			Vec3 position;
			position.x = mesh.worldTransform[3][0];
			position.y = mesh.worldTransform[3][1];
			position.z = mesh.worldTransform[3][2];
			Vec3 scale;
			scale.x = glm::length(Vec3(mesh.worldTransform[0]));
			scale.y = glm::length(Vec3(mesh.worldTransform[1]));
			scale.z = glm::length(Vec3(mesh.worldTransform[2]));
			float scaleFactor = glm::max(scale.z, glm::max(scale.x, scale.y));

			if (mesh.cull && gRenderCullingSphere)
			{
				DebugRenderer::addSphere({ 1, 1, 0 }, mesh.boundingSphereRadius * scaleFactor, position);
			}

			//Frustum culling
			if (mesh.cull && isOnFrustum(frustum, position, mesh.boundingSphereRadius))
			{

				gBufferShader->uploadModel(mesh.worldTransform);
				glBindVertexArray(mesh.vao);
				glDrawElements(GL_TRIANGLES, mesh.vertexCount, mesh.eboDataType, nullptr);

			}
			else
			{
				gBufferShader->uploadModel(mesh.worldTransform);
				glBindVertexArray(mesh.vao);
				glDrawElements(GL_TRIANGLES, mesh.vertexCount, mesh.eboDataType, nullptr);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, Window::getWidth(), Window::getHeight());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPoisitonTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColorTex);


		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(gQuadVAO);

		//Ambient pass
		gAmbientRenderer->render();

		//Directional pass
		System& directionalSystem = ECS::getSystem(SystemType::DIRECTIONAL);
		for (auto e : directionalSystem.entities)
		{
			DirectionalLight::Component* pLight = (DirectionalLight::Component*)ECS::getComponent(e, ComponentType::DIRECTIONAL_LIGHT);

			gDirectionalRenderer->render(pLight->direction, pLight->color, pLight->intensity, { camera.x, camera.y, camera.z });
		}

		//Point pass
		System& pointSystem = ECS::getSystem(SystemType::POINT);
		for (auto e : pointSystem.entities)
		{
			PointLight::Component* pLight = (PointLight::Component*)ECS::getComponent(e, ComponentType::POINT_LIGHT);
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);

			gPointRenderer->render(pLight->color,
				{ pTransform->x, pTransform->y, pTransform->z },
				pLight->intensity,
				pLight->constant,
				pLight->linear,
				pLight->exponent,
				{ camera.x, camera.y, camera.z });

		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);

		gMeshData.clear();
	}

	static void updateGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale)
	{
		UInt32 width = inWidth * scale;
		UInt32 height = inHeight * scale;
		glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
		glBindTexture(GL_TEXTURE_2D, gPoisitonTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPoisitonTex, 0);

		glBindTexture(GL_TEXTURE_2D, gNormalTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalTex, 0);

		glBindTexture(GL_TEXTURE_2D, gColorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorTex, 0);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static void initQuad()
	{
		constexpr float data[] =
		{
			 -1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 1.0f, 1.0f,
		};

		glGenVertexArrays(1, &gQuadVAO);
		glGenBuffers(1, &gQuadVBO);

		glBindVertexArray(gQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, gQuadVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
}






