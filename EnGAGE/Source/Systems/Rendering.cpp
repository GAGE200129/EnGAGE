#include "pch.hpp"
#include "Rendering.hpp"

#include "Core/Model.hpp"
#include "Core/Window.hpp"

#include <glad/glad.h>

static void renderModel(const Ref<Core::Model>& model, const Core::Node& node);

Core::Rendering::Rendering() :
	mCamera({})
{
	mCamera.fov = 75.0f;
	mCamera.near = 1.0f;
	mCamera.far = 100.0f;
}

void Core::Rendering::render()
{
	auto buildViewMat = [](const Camera& camera) -> glm::mat4x4
	{
		glm::mat4x4 result;
		result = glm::rotate(glm::mat4x4(1.0f), -glm::radians(camera.pitch), {1, 0, 0});
		result = glm::rotate(result, -glm::radians(camera.yaw), {0, 1, 0});
		result = glm::translate(result, { -camera.x, -camera.y, -camera.z });
		return result;
	};


	unsigned int windowWidth = Window::getWidth();
	unsigned int windowHeight = Window::getHeight();
	auto viewMat = buildViewMat(mCamera);
	auto projMat = glm::perspective(glm::radians(mCamera.fov), (float)windowWidth / (float)windowHeight, mCamera.near, mCamera.far);

	if (Window::resized())
	{
		glViewport(0, 0, windowWidth, windowHeight);
	}

	glClear(GL_COLOR_BUFFER_BIT);
	mShader.bind();
	mShader.uploadViewMat(viewMat);
	mShader.uploadProjMat(projMat);
	for (const auto& component : mComponents)
	{
		if (auto ptr = component->mModel.lock())
		{
			unsigned int rootNodeIndex = ptr->rootNodeIndex;
			renderModel(ptr, ptr->nodes[rootNodeIndex]);
		}
	}
}

void Core::Rendering::cleanup()
{
	mShader.cleanup();
}

static void renderModel(const Ref<Core::Model>& model, const Core::Node& node)
{
	if (node.meshIndex != -1)
	{
		const auto& mesh = model->meshes[node.meshIndex];
		for (const auto& primitive : mesh.primitives)
		{
			glBindVertexArray(primitive.vao);
			glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
		}
	}


	for (const auto& child : node.children)
	{
		renderModel(model, model->nodes[child]);
	}
}
