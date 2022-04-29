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
		PHYSICS_UPDATE_RIGID_BODY,
		PHYSICS_INIT_COLLISION_SHAPE,
		REMOVE_SCRIPT,
		REMOVE_RIGID_BODY,
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
		unsigned char message[Messenger::BUFFER_SIZE];
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


	const MessageData& getMessageData(MessageType type);

	

}