#include "pch.hpp"
#include "DebugCamera.hpp"

#include "InputCodes.hpp"

void Core::DebugCamera::onMessage(bool imguiWantCaptureMouse, const Message* pMessage)
{
	if (auto buttonClicked = Messenger::messageCast<MessageType::BUTTON_PRESSED, ButtonPressedMessage>(pMessage))
	{
		if (buttonClicked->buttonCode == 1)
		{
			mRotateCamera = true;
		}
	}
	else if (auto buttonReleased = Messenger::messageCast<MessageType::BUTTON_RELEASED, ButtonReleasedMessage>(pMessage))
	{
		if (buttonReleased->buttonCode == 1)
		{
			mRotateCamera = false;
		}
	}
	else if (auto cursorMoved = Messenger::messageCast<MessageType::CURSOR_MOVED, CursorMovedMessage>(pMessage))
	{
		if (mRotateCamera && !imguiWantCaptureMouse)
		{
			mCamera.yaw -= cursorMoved->dx * MOUSE_SENSITIVITY;
			mCamera.pitch -= cursorMoved->dy * MOUSE_SENSITIVITY;

			if (mCamera.pitch > 90.0f) mCamera.pitch = 90.0;
			else if (mCamera.pitch < -90.0f) mCamera.pitch = -90.0f;

			if (mCamera.yaw > 360.0f) mCamera.yaw = -360.0f;
			else if (mCamera.yaw < -360.0f) mCamera.yaw = 360.0f;
		}
	}
	else if (auto keyPressedMessage = Messenger::messageCast<MessageType::KEY_PRESSED, KeyPressedMessage>(pMessage))
	{
		auto key = keyPressedMessage->keyCode;
		if (key == InputCodes::KEY_W)
			mForward = true;
		if (key == InputCodes::KEY_S)
			mBackward = true;
		if (key == InputCodes::KEY_D)
			mRight = true;
		if (key == InputCodes::KEY_A)
			mLeft = true;
		if (key == InputCodes::KEY_SPACE)
			mUp = true;
		if (key == InputCodes::KEY_LEFT_SHIFT)
			mDown = true;

	}
	else if (auto keyReleased = Messenger::messageCast<MessageType::KEY_RELEASED, KeyReleasedMessage>(pMessage))
	{
		auto key = keyReleased->keyCode;
		if (key == InputCodes::KEY_W)
			mForward = false;
		if (key == InputCodes::KEY_S)
			mBackward = false;
		if (key == InputCodes::KEY_D)
			mRight = false;
		if (key == InputCodes::KEY_A)
			mLeft = false;
		if (key == InputCodes::KEY_SPACE)
			mUp = false;
		if (key == InputCodes::KEY_LEFT_SHIFT)
			mDown = false;
	}
}

void Core::DebugCamera::update(F32 delta)
{
	mCamera.fov = CAM_FOV;
	mCamera.near = CAM_NEAR;
	mCamera.far = CAM_FAR;
	mDirection = { 0, 0, 0 };

	F32 sinYaw = glm::sin(glm::radians(mCamera.yaw));
	F32 sin90Yaw = glm::sin(glm::radians(mCamera.yaw + 90.0f));
	F32 cosYaw = glm::cos(glm::radians(mCamera.yaw));
	F32 cos90Yaw = glm::cos(glm::radians(mCamera.yaw + 90.0f));

	if (mForward)
	{
		mDirection.x -= sinYaw;
		mDirection.z -= cosYaw;
	}

	if (mBackward)
	{
		mDirection.x += sinYaw;
		mDirection.z += cosYaw;
	}

	if (mRight)
	{
		mDirection.x += sin90Yaw;
		mDirection.z += cos90Yaw;
	}

	if (mLeft)
	{
		mDirection.x -= sin90Yaw;
		mDirection.z -= cos90Yaw;
	}

	if (mUp)
	{
		mDirection.y += 1;
	}

	if (mDown)
	{
		mDirection.y -= 1;
	}

	F32 mDirection_length_squared = glm::length2(mDirection);

	if (mDirection_length_squared != 0.0f)
	{
		mDirection = glm::normalize(mDirection);
	}

	mCamera.x += mDirection.x * delta * CAM_SPEED;
	mCamera.y += mDirection.y * delta * CAM_SPEED;
	mCamera.z += mDirection.z * delta * CAM_SPEED;
}
