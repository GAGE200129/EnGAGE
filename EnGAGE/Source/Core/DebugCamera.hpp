#pragma once

#include "pch.hpp"
#include "Camera.hpp"
#include "Messenger.hpp"

namespace Core
{
	class DebugCamera
	{
	public:
		DebugCamera(Camera& camera) : mCamera(camera) {}

		void onMessage(bool imguiWantCaptureMouse, const Message* pMessage);
		void update(F32 delta);
	private:
		static constexpr F32 MOUSE_SENSITIVITY = 0.3f;
		static constexpr F32 CAM_NEAR = 1.0f;
		static constexpr F32 CAM_FAR = 1000.0f;
		static constexpr F32 CAM_FOV = 70.0f;
		static constexpr F32 CAM_SPEED = 7;

		Camera& mCamera;
		bool mRotateCamera = false;
		Vec3 mDirection = { 0, 0, 0 };
		bool mForward = false;
		bool mBackward = false;
		bool mLeft = false;
		bool mRight = false;
		bool mUp = false;
		bool mDown = false;
	};
}