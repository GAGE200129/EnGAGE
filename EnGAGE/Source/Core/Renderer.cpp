#include "pch.hpp"
#include "Renderer.hpp"

#include "ECS.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"

#include <glad/glad.h>

namespace Core::Renderer
{
	static void buildProjViewMatrix(const Camera& camera, unsigned int width, unsigned int height, glm::mat4x4& outProjMat, glm::mat4x4& outViewMat);
	static void processNode(const Model& model, const Node& node, glm::mat4x4 accumulatedTransform);

	static Camera gCamera;
	static glm::mat4x4 gProj;
	static glm::mat4x4 gView;
	static Scope<Shader> gShader;
	static int projLoc, viewLoc, modelLoc;

	void init()
	{
		gShader = createScope<Core::Shader>();
		gShader->loadVertexShader("Resources/Shaders/test_VS.glsl");
		gShader->loadFragmentShader("Resources/Shaders/test_FS.glsl");
		gShader->compile();

		projLoc = gShader->registerUniform("uProj");
		viewLoc = gShader->registerUniform("uView");
		modelLoc = gShader->registerUniform("uModel");
	}

	void shutdown()
	{
		gShader->cleanup();
	}

	void render()
	{
		ECS::System& system = ECS::getSystem(ECS::SystemType::RENDERER);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Window::getWidth(), Window::getHeight());
		buildProjViewMatrix(gCamera, Window::getWidth(), Window::getHeight(), gProj, gView);
		gShader->bind();
		gShader->uploadMat4x4(projLoc, gProj);
		gShader->uploadMat4x4(viewLoc, gView);

		for (unsigned int e : system.entities)
		{
			ECS::TransformComponent* pTransform = (ECS::TransformComponent*)ECS::getComponent(e, ECS::ComponentType::TRANSFORM);
			ECS::ModelRendererComponent* pModelComp = (ECS::ModelRendererComponent*)ECS::getComponent(e, ECS::ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				Model& model = *pModelComp->pModel;
				glm::mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(model, model.nodes[model.rootNodeIndex], modelMat);
			}
		}
	}

	Camera& getCamera()
	{
		return gCamera;
	}

	static void buildProjViewMatrix(const Camera& camera, unsigned int width, unsigned int height, glm::mat4x4& outProjMat, glm::mat4x4& outViewMat)
	{
		outViewMat = glm::rotate(glm::mat4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		outViewMat = glm::rotate(outViewMat, -glm::radians(camera.yaw), { 0, 1, 0 });
		outViewMat = glm::translate(outViewMat, { -camera.x, -camera.y, -camera.z });

		outProjMat = glm::perspective(glm::radians(camera.fov), (float)width / (float)height, camera.near, camera.far);
	}

	static void processNode(const Model& model, const Node& node, glm::mat4x4 accumulatedTransform)
	{
		accumulatedTransform = glm::translate(accumulatedTransform, node.position);
		accumulatedTransform *= glm::toMat4(node.rotation);
		accumulatedTransform = glm::scale(accumulatedTransform, node.scale);


		if (node.meshIndex != -1)
		{
			const Core::Mesh& mesh = model.meshes[node.meshIndex];

			for (const auto& primitive : mesh.primitives)
			{
				gShader->uploadMat4x4(modelLoc, accumulatedTransform);
				glBindVertexArray(primitive.vao);
				glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
			}
		}
		for (const auto& child : node.children)
		{
			processNode(model, model.nodes[child], accumulatedTransform);
		}
	}
}






