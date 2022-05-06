#pragma once

#include "Messenger.hpp"

namespace Core::DebugRenderer
{
	void init(unsigned int width, unsigned int height);
	void onMessage(const Message* pMessage);
	void drawAllAABB();
	void addLine(const glm::vec3& color, const glm::vec3& begin, const glm::vec3& end);
	void addBox(const glm::vec3& color, const glm::vec3& min, const glm::vec3& max, const glm::mat4x4& modelTransform);
	void render();
	void shutdown();
}