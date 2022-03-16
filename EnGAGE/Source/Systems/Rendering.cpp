#include "pch.hpp"
#include "Rendering.hpp"

#include "Core/Model.hpp"
#include "Core/Window.hpp"

#include <glad/glad.h>

#include "Core/GameObject.hpp"


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
	glClearColor(0.1f, 0.1f, 0.1f, 1);
	mShader.bind();
	mShader.uploadViewMat(viewMat);
	mShader.uploadProjMat(projMat);
	for (const auto& component : mComponents)
	{	
		if (const Model* ptr = component->pModel)
		{
			glm::mat4x4 modelMat = glm::translate(glm::mat4x4(1.0f), component->mParent->mPosition);
			modelMat *= glm::toMat4(component->mParent->mRotation);
			modelMat = glm::scale(modelMat, component->mParent->mScale);
			unsigned int rootNodeIndex = ptr->rootNodeIndex;
			renderModel(*ptr, ptr->nodes[rootNodeIndex], modelMat);
		}
	}
}

void Core::Rendering::cleanup()
{
	mShader.cleanup();
}

void Core::Rendering::renderModel(const Model& model, const Node& node, glm::mat4x4 accumulatedTransform)
{
	//Apply node's local transform to model transform
	accumulatedTransform = glm::translate(accumulatedTransform, node.position);
	accumulatedTransform *= glm::toMat4(node.rotation);
	accumulatedTransform = glm::scale(accumulatedTransform, node.scale);

	//Check if node's mesh present
	if (node.meshIndex != -1)
	{
		mShader.uploadModelMat(accumulatedTransform);
		const auto& mesh = model.meshes[node.meshIndex];
		for (const auto& primitive : mesh.primitives)
		{
			const auto& material = model.materials[primitive.materialIndex];
			const auto& baseColorTexture = model.textures[material.baseColorIndex];

			glActiveTexture(0);
			glBindTexture(GL_TEXTURE_2D, baseColorTexture.textureID);

			glBindVertexArray(primitive.vao);
			glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
		}
	}


	for (const auto& child : node.children)
	{
		renderModel(model, model.nodes[child], accumulatedTransform);
	}
}
