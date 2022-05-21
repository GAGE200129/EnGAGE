#pragma once

#include "Core/Messenger.hpp"
#include "Core/Camera.hpp"

namespace Core::DebugRenderer
{
	void init();
	void drawAllAABB();
	void addLine(const Vec3& color, const Vec3& begin, const Vec3& end);
	void addBox(const Vec3& color, const Vec3& min, const Vec3& max, const Mat4x4& modelTransform);
	void addSphere(const Vec3& color, float radius, const Vec3& position);
	void render(const Camera& camera);
	void shutdown();
}