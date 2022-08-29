#include "pch.hpp"
#include "Animation.hpp"

#include "Core/ECS/ECS.hpp"
#include "Core/ECS/Animator.hpp"
#include "Core/ECS/ModelRenderer.hpp"

#include <glm/gtx/quaternion.hpp>

namespace Core::Animation
{
	void update(float delta)
	{
		auto& system = ECS::getSystem(SystemType::ANIMATION);

		for (const auto& entity : system.entities)
		{
			Animator::Component* pAnimator = (Animator::Component*)ECS::getComponent(entity, ComponentType::ANIMATOR);
			ModelRenderer::Component* pModel = (ModelRenderer::Component*)ECS::getComponent(entity, ComponentType::MODEL_RENDERER);

			pAnimator->currentTime += delta;

			const AnimationData& currentAnimation = pModel->pModel->animations[0];
			const std::vector<Channel>& channels = currentAnimation.channels;
			for (int i = 0; i < channels.size(); i++)
			{
				const Channel& currentChannel = channels[i];
				const Sampler& sampler = currentAnimation.samplers[currentChannel.samplerIndex];
				glm::mat4x4& matrix = pModel->boneMatrices[currentChannel.node];
				matrix = glm::mat4x4(1.0f);
				matrix *= pModel->pModel->nodes[currentChannel.node].inverseBindTransform;

				//Get data for factor
				size_t currentIndex = 0, nextIndex = 1;
				float currentTimePoint = 0.0f, nextTimePoint = sampler.input[1];
				float finalSampler = sampler.input[sampler.input.size() - 1];
				for (int i = 0; i < (sampler.input.size() - 1); i++)
				{
					float currentSampler = sampler.input[i];
					float nextSampler = sampler.input[i + 1];

					if (pAnimator->currentTime > finalSampler)
						break;

					if (pAnimator->currentTime > currentSampler)
					{
						currentTimePoint = currentSampler;
						nextTimePoint = nextSampler;

						currentIndex = i;
						nextIndex = i + 1;
						break;
					}
				}

				//Calculate factor
				float factor = (pAnimator->currentTime - currentTimePoint) / (nextTimePoint - currentTimePoint);

				//Do the transform
				switch (currentChannel.path)
				{
				case Core::Channel::Path::TRANSLATION:
				{
					glm::vec3 currentPosition, nextPosition, position;

					currentPosition.x = sampler.output[currentIndex + 0];
					currentPosition.y = sampler.output[currentIndex + 1];
					currentPosition.z = sampler.output[currentIndex + 2];
					nextPosition.x = sampler.output[nextIndex + 0];
					nextPosition.y = sampler.output[nextIndex + 1];
					nextPosition.z = sampler.output[nextIndex + 2];
					position = glm::mix(currentPosition, nextPosition, factor);


					matrix = glm::translate(matrix, position);
					break;
				}
				case Core::Channel::Path::ROTATION:
				{
					glm::quat currentRotation, nextRotattion, rotation;

					currentRotation.x = sampler.output[currentIndex + 0];
					currentRotation.y = sampler.output[currentIndex + 1];
					currentRotation.z = sampler.output[currentIndex + 2];
					currentRotation.w = sampler.output[currentIndex + 3];

					nextRotattion.x = sampler.output[nextIndex + 0];
					nextRotattion.y = sampler.output[nextIndex + 1];
					nextRotattion.z = sampler.output[nextIndex + 2];
					nextRotattion.w = sampler.output[nextIndex + 3];

					rotation = glm::mix(currentRotation, nextRotattion, factor);

					matrix *= glm::toMat4(rotation);
					break;
				}
				case Core::Channel::Path::SCALE:
				{
					glm::vec3 scale;

					scale.x = sampler.output[0];
					scale.y = sampler.output[1];
					scale.z = sampler.output[2];

					matrix = glm::translate(matrix, scale);
					break;
				}
				}

				

			}
		}
	}
}
