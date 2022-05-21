#pragma once

#include "MessageConstants.hpp"

struct lua_State;
namespace Core
{
	enum class MessageType : unsigned int
	{
		KEY_PRESSED,
		KEY_RELEASED,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		CURSOR_MOVED,
		TOGGLE_CURSOR,
		WINDOW_RESIZED,
		WINDOW_RENAMED,
		PHYSICS_UPDATE_RIGID_BODY,
		PHYSICS_INIT_COLLISION_SHAPE,
		REMOVE_SCRIPT,
		REMOVE_RIGID_BODY,
		RENDERER_TOGGLE_CULLING_SPHERE,
		RENDERER_SET_SCALE,
		EDITOR_TOGGLE,
		COUNT
	};

	struct MessageData
	{
		const char* name;
		unsigned int size;
	};

	struct Message
	{
		MessageType type;
		Byte message[Messenger::BUFFER_SIZE];
	};

	struct KeyPressedMessage
	{
		unsigned int keyCode;
	};

	struct KeyReleasedMessage
	{
		unsigned int keyCode;
	};

	struct ButtonPressedMessage
	{
		unsigned int buttonCode;
	};

	struct ButtonReleasedMessage
	{
		unsigned int buttonCode;
	};

	struct CursorMovedMessage
	{
		float x, y, dx, dy;
	};

	struct WindowResizedMessage
	{
		unsigned int width, height;
	};

	struct PhysicsUpdateRigidBodyMessage
	{
		btRigidBody* body;
	};

	struct PhysicsInitCollisionShapeMessage
	{
		btRigidBody* body;
		unsigned int type;
		unsigned char arguments[50];
	};

	struct RemoveScriptMessage
	{
		lua_State* L;
	};

	struct RemoveRigidBodyMessage
	{
		btRigidBody* body;
	};
	struct WindowRenamedMessage
	{
		char name[Messenger::BUFFER_SIZE];
	};

	struct RendererSetScaleMessage
	{
		float scale;
	};
	const MessageData& getMessageData(MessageType type);
}