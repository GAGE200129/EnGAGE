#include "pch.hpp"
#include "DebugCamera.hpp"


#include "Core/Input.hpp"
#include "Core/InputCodes.hpp"
#include "Core/Camera.hpp"
#include "Core/GameEngine.hpp"
#include "Core/Window.hpp"

namespace Core::DebugCamera
{
	enum class OrthoMode
	{
		NONE,
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	struct
	{
		bool leftMouseClicked = false;
		Vec3 direction = { 0, 0, 0 };
		bool forward = false;
		bool backward = false;
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
		F32 orthoScale = 10;
		OrthoMode orthoMode = OrthoMode::NONE;
	} gDebugCamState;

	static constexpr F32 MOUSE_SENSITIVITY = 0.3f;
	static constexpr F32 CAM_NEAR = 1.0f;
	static constexpr F32 CAM_FAR = 1000.0f;
	static constexpr F32 CAM_FOV = 70.0f;
	static constexpr F32 CAM_SPEED = 7;

	void onMessage(const Message* pMessage)
	{
		if (pMessage->type == MessageType::SCROLL_UP && GameEngine::getDebugCamera().mode == Camera::Mode::ORTHOGRAPHIC)
		{
			gDebugCamState.orthoScale -= 1;
			if (gDebugCamState.orthoScale < 1) gDebugCamState.orthoScale = 1;
		}
		else if (pMessage->type == MessageType::SCROLL_DOWN && GameEngine::getDebugCamera().mode == Camera::Mode::ORTHOGRAPHIC)
		{
			gDebugCamState.orthoScale += 1;
			if (gDebugCamState.orthoScale < 1) gDebugCamState.orthoScale = 1;
		}
		else if (auto buttonClicked = Messenger::messageCast<MessageType::BUTTON_PRESSED, ButtonPressedMessage>(pMessage))
		{
			if (buttonClicked->buttonCode == 1)
			{
				gDebugCamState.leftMouseClicked = true;
			}
		}
		else if (auto buttonReleased = Messenger::messageCast<MessageType::BUTTON_RELEASED, ButtonReleasedMessage>(pMessage))
		{
			if (buttonReleased->buttonCode == 1)
			{
				gDebugCamState.leftMouseClicked = false;
			}
		}
		else if (auto cursorMoved = Messenger::messageCast<MessageType::CURSOR_MOVED, CursorMovedMessage>(pMessage))
		{
			auto& camera = GameEngine::getDebugCamera();
			if (gDebugCamState.leftMouseClicked && camera.mode != Camera::Mode::ORTHOGRAPHIC)
			{
				camera.yaw -= cursorMoved->dx * MOUSE_SENSITIVITY;
				camera.pitch -= cursorMoved->dy * MOUSE_SENSITIVITY;

				if (camera.pitch > 90.0f) camera.pitch = 90.0;
				else if (camera.pitch < -90.0f) camera.pitch = -90.0f;

				if (camera.yaw > 360.0f) camera.yaw = -360.0f;
				else if (camera.yaw < -360.0f) camera.yaw = 360.0f;
			}
			else if (camera.mode == Camera::Mode::ORTHOGRAPHIC && gDebugCamState.leftMouseClicked)
			{
				F32 aspectRatio = (F32)Window::getWidth() / (F32)Window::getHeight();
				F32 orthoScale2 = gDebugCamState.orthoScale * 2.0f;
				F32 dxNormalized = (cursorMoved->dx / Window::getWidth());
				F32 dyNormalized = (cursorMoved->dy / Window::getHeight());
				if (gDebugCamState.orthoMode == OrthoMode::BACK)
				{
					camera.x -= dxNormalized * aspectRatio * orthoScale2;
					camera.y += dyNormalized * orthoScale2;
				}
				else if (gDebugCamState.orthoMode == OrthoMode::FRONT)
				{
					camera.x += dxNormalized * aspectRatio * orthoScale2;
					camera.y += dyNormalized * orthoScale2;
				}
				else if (gDebugCamState.orthoMode == OrthoMode::LEFT)
				{
					camera.z -= dxNormalized * aspectRatio * orthoScale2;
					camera.y += dyNormalized * orthoScale2;
				}
				else if (gDebugCamState.orthoMode == OrthoMode::RIGHT)
				{
					camera.z += dxNormalized * aspectRatio * orthoScale2;
					camera.y += dyNormalized * orthoScale2;
				}

				else if (gDebugCamState.orthoMode == OrthoMode::UP)
				{
					camera.x += dxNormalized * aspectRatio * orthoScale2;
					camera.z -= dyNormalized * orthoScale2;
				}
				else if (gDebugCamState.orthoMode == OrthoMode::DOWN)
				{
					camera.x -= dxNormalized * aspectRatio * orthoScale2;
					camera.z -= dyNormalized * orthoScale2;
				}
			}
		}
		else if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
		{
			auto key = keyPressedMessage->keyCode;
			//Toggle ortho perspective
			if (key == InputCodes::KEY_GRAVE_ACCENT)
				gDebugCamState.orthoMode = OrthoMode::NONE;
			else if (key == InputCodes::KEY_1)
				gDebugCamState.orthoMode = OrthoMode::FRONT;
			else if (key == InputCodes::KEY_2)
				gDebugCamState.orthoMode = OrthoMode::BACK;
			else if (key == InputCodes::KEY_3)
				gDebugCamState.orthoMode = OrthoMode::LEFT;
			else if (key == InputCodes::KEY_4)
				gDebugCamState.orthoMode = OrthoMode::RIGHT;
			else if (key == InputCodes::KEY_5)
				gDebugCamState.orthoMode = OrthoMode::UP;
			else if (key == InputCodes::KEY_6)
				gDebugCamState.orthoMode = OrthoMode::DOWN;
			//Move camera
			else if (key == InputCodes::KEY_W)
				gDebugCamState.forward = true;
			else if (key == InputCodes::KEY_S)
				gDebugCamState.backward = true;
			else if (key == InputCodes::KEY_D)
				gDebugCamState.right = true;
			else if (key == InputCodes::KEY_A)
				gDebugCamState.left = true;
			else if (key == InputCodes::KEY_SPACE)
				gDebugCamState.up = true;
			else if (key == InputCodes::KEY_LEFT_SHIFT)
				gDebugCamState.down = true;

		}
		else if (auto keyReleased = Messenger::messageCast<MessageType::KEY_RELEASED, KeyReleasedMessage>(pMessage))
		{
			auto key = keyReleased->keyCode;

			if (key == InputCodes::KEY_W)
				gDebugCamState.forward = false;
			if (key == InputCodes::KEY_S)
				gDebugCamState.backward = false;
			if (key == InputCodes::KEY_D)
				gDebugCamState.right = false;
			if (key == InputCodes::KEY_A)
				gDebugCamState.left = false;
			if (key == InputCodes::KEY_SPACE)
				gDebugCamState.up = false;
			if (key == InputCodes::KEY_LEFT_SHIFT)
				gDebugCamState.down = false;
		}
	}
	void update(F32 delta)
	{
		auto& camera = GameEngine::getDebugCamera();
		camera.fov = DebugCamera::CAM_FOV;
		camera.near = DebugCamera::CAM_NEAR;
		camera.far = DebugCamera::CAM_FAR;
		gDebugCamState.direction = { 0, 0, 0 };

		F32 sinYaw = glm::sin(glm::radians(camera.yaw));
		F32 sin90Yaw = glm::sin(glm::radians(camera.yaw + 90.0f));
		F32 cosYaw = glm::cos(glm::radians(camera.yaw));
		F32 cos90Yaw = glm::cos(glm::radians(camera.yaw + 90.0f));

		if (gDebugCamState.forward)
		{
			gDebugCamState.direction.x -= sinYaw;
			gDebugCamState.direction.z -= cosYaw;
		}

		if (gDebugCamState.backward)
		{
			gDebugCamState.direction.x += sinYaw;
			gDebugCamState.direction.z += cosYaw;
		}

		if (gDebugCamState.right)
		{
			gDebugCamState.direction.x += sin90Yaw;
			gDebugCamState.direction.z += cos90Yaw;
		}

		if (gDebugCamState.left)
		{
			gDebugCamState.direction.x -= sin90Yaw;
			gDebugCamState.direction.z -= cos90Yaw;
		}

		if (gDebugCamState.up)
		{
			gDebugCamState.direction.y += 1;
		}

		if (gDebugCamState.down)
		{
			gDebugCamState.direction.y -= 1;
		}

		if (glm::length2(gDebugCamState.direction) != 0.0f)
		{
			gDebugCamState.direction = glm::normalize(gDebugCamState.direction);
		}

		if (gDebugCamState.orthoMode != DebugCamera::OrthoMode::NONE)
		{
			F32 aspectRatio = (F32)Window::getWidth() / (F32)Window::getHeight();
			camera.mode = Camera::Mode::ORTHOGRAPHIC;
			camera.minX = -gDebugCamState.orthoScale * aspectRatio;
			camera.maxX = gDebugCamState.orthoScale * aspectRatio;
			camera.minY = -gDebugCamState.orthoScale;
			camera.maxY = gDebugCamState.orthoScale;
			camera.minZ = -gDebugCamState.orthoScale - 100;
			camera.maxZ = gDebugCamState.orthoScale + 100;
			if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::FRONT)
			{
				camera.pitch = 0;
				camera.yaw = 180;
			}
			else if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::BACK)
			{
				camera.pitch = 0;
				camera.yaw = 0;
			}
			else if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::LEFT)
			{
				camera.pitch = 0;
				camera.yaw = -90;
			}
			else if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::RIGHT)
			{
				camera.pitch = 0;
				camera.yaw = 90;
			}
			else if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::UP)
			{
				camera.pitch = 90.0f;
				camera.yaw = 180.0f;
			}
			else if (gDebugCamState.orthoMode == DebugCamera::OrthoMode::DOWN)
			{
				camera.pitch = -90.0f;
				camera.yaw = 0;
			}
		}
		else
		{
			camera.mode = Camera::Mode::PERSPECTIVE;
			camera.x += gDebugCamState.direction.x * delta * DebugCamera::CAM_SPEED;
			camera.y += gDebugCamState.direction.y * delta * DebugCamera::CAM_SPEED;
			camera.z += gDebugCamState.direction.z * delta * DebugCamera::CAM_SPEED;
		}
	}
}
