#include "pch.hpp"
#include "Renderer.hpp"

#include "ECS.hpp"

#include <glad/glad.h>

static void processNode(Core::Model* model, const Core::Node& node);

void Core::Renderer::init()
{
}

void Core::Renderer::shutdown()
{
}

void Core::Renderer::render()
{	
	ECS::System* system = ECS::getSystem(ECS::SystemType::RENDERER);
	glClear(GL_COLOR_BUFFER_BIT);
	for (unsigned int i = 0; i < system->entityCount; i++)
	{
		unsigned int e = system->entities[i];

		ECS::TransformComponent* pTransform = (ECS::TransformComponent*)ECS::getComponent(e, ECS::ComponentType::TRANSFORM);
		ECS::ModelRendererComponent* pModel = (ECS::ModelRendererComponent*)ECS::getComponent(e, ECS::ComponentType::MODEL_RENDERER);
		if (pModel->pModel)
		{
			processNode(pModel->pModel, pModel->pModel->nodes[pModel->pModel->rootNodeIndex]);
		}
	}
}

static void processNode(Core::Model* model, const Core::Node& node)
{
	if (node.meshIndex != -1)
	{
		Core::Mesh& mesh = model->meshes[node.meshIndex];

		for (const auto& primitive : mesh.primitives)
		{
			glBindVertexArray(primitive.vao);
			glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
		}
	}
	for (const auto& child : node.children)
	{
		processNode(model, model->nodes[child]);
	}
}
