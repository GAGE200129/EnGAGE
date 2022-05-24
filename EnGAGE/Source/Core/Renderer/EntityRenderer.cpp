#include "pch.hpp"
#include "EntityRenderer.hpp"

#include "Core/Math.hpp"
#include "Core/Model.hpp"
#include "Core/ECS.hpp"
#include "Core/Components/Transform.hpp"
#include "Core/Components/ModelRenderer.hpp"
#include "DebugRenderer.hpp"

#include <glad/glad.h>

namespace Core
{
	void EntityRenderer::render(GBuffer& gBuffer, const Camera& camera, bool renderCullingSphere)
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

		std::function<void(const Math::Frustum& frustum, const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)> processNode;
		processNode = [&](const Math::Frustum& frustum, const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)
		{
			accumulatedTransform = glm::translate(accumulatedTransform, node.position);
			accumulatedTransform *= glm::toMat4(node.rotation);
			accumulatedTransform = glm::scale(accumulatedTransform, node.scale);

			if (node.meshIndex != -1)
			{
				const Core::Mesh& mesh = model->meshes[node.meshIndex];
				for (const auto& primitive : mesh.primitives)
				{
					

					Vec3 position;
					position.x = accumulatedTransform[3][0];
					position.y = accumulatedTransform[3][1];
					position.z = accumulatedTransform[3][2];
					Vec3 scale;
					scale.x = glm::length(Vec3(accumulatedTransform[0]));
					scale.y = glm::length(Vec3(accumulatedTransform[1]));
					scale.z = glm::length(Vec3(accumulatedTransform[2]));
					float scaleFactor = glm::max(scale.z, glm::max(scale.x, scale.y));

					if (renderCullingSphere)
					{
						DebugRenderer::addSphere({ 1, 1, 0 }, mesh.boundingSphereRadius * scaleFactor, position);
					}

					//Frustum culling
					if (camera.mode == Camera::Mode::ORTHOGRAPHIC || isOnFrustum(frustum, position, mesh.boundingSphereRadius))
					{

						gBuffer.uploadModel(accumulatedTransform);
						glBindVertexArray(primitive.vao);
						glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);

					}
				}
			}
			for (const auto& child : node.children)
			{
				processNode(frustum, model, model->nodes[child], accumulatedTransform);
			}
		};

		//Render all to Geometry buffer
		
		Math::Frustum frustum = Math::createFrustum(camera);
		System& system = ECS::getSystem(SystemType::RENDERER);
		for (auto e : system.entities)
		{
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);
			ModelRenderer::Component* pModelComp = (ModelRenderer::Component*)ECS::getComponent(e, ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				const Model* pModel = pModelComp->pModel;
				glm::mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(frustum, pModel, pModel->nodes[pModel->rootNodeIndex], modelMat);
			}
		}
	}
}
