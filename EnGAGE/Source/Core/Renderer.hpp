#pragma once

#include "Messenger.hpp"
#include "Camera.hpp"

namespace Core::Renderer
{
	void init(UInt32 currentWidth, UInt32 currentHeight);
	void onMessage(const Message* pMessage);
	void shutdown();
	void submitMesh(UInt32 vao, UInt64 vertexCount, UInt32 eboDataType, const Mat4x4& worldTransform = Mat4x4(1.0f), bool cull = false, float sphereRadius = 0.0f);
	void render(const Camera& camera);
}