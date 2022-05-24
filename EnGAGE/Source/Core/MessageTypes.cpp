#include "pch.hpp"
#include "MessageTypes.hpp"

#define TO_SWITCH(x) case MessageType::x: return MessageDataEnum<MessageType::x>

namespace Core
{
	template<MessageType> MessageData MessageDataEnum;
	template<> MessageData MessageDataEnum<MessageType::SCROLL_UP				> = { "SCROLL_UP", 0 };
	template<> MessageData MessageDataEnum<MessageType::SCROLL_DOWN				> = { "SCROLL_DOWN", 0 };
	template<> MessageData MessageDataEnum<MessageType::KEY_PRESSED				   > = {"KEY_PRESSED", sizeof(KeyPressedMessage)};
	template<> MessageData MessageDataEnum<MessageType::KEY_RELEASED			   > = {"KEY_RELEASED", sizeof(KeyReleasedMessage)};
	template<> MessageData MessageDataEnum<MessageType::BUTTON_PRESSED			   > = {"BUTTON_PRESSED", sizeof(ButtonPressedMessage)};
	template<> MessageData MessageDataEnum<MessageType::BUTTON_RELEASED			   > = {"BUTTON_RELEASED", sizeof(ButtonReleasedMessage)};
	template<> MessageData MessageDataEnum<MessageType::CURSOR_MOVED			   > = {"CURSOR_MOVED", sizeof(CursorMovedMessage)};
	template<> MessageData MessageDataEnum<MessageType::TOGGLE_CURSOR			   > = {"TOGGLE_CURSOR", 0};
	template<> MessageData MessageDataEnum<MessageType::WINDOW_RESIZED			   > = {"WINDOW_RESIZED", sizeof(WindowResizedMessage)};
	template<> MessageData MessageDataEnum<MessageType::WINDOW_RENAMED			   > = {"WINDOW_RENAMED", sizeof(WindowRenamedMessage)};
	template<> MessageData MessageDataEnum<MessageType::PHYSICS_UPDATE_RIGID_BODY  > = {"PHYSICS_UPDATE_RIGID_BODY", sizeof(PhysicsUpdateRigidBodyMessage)};
	template<> MessageData MessageDataEnum<MessageType::PHYSICS_INIT_COLLISION_SHAPE> = {"PHYSICS_INIT_COLLISION_SHAPE", sizeof(PhysicsInitCollisionShapeMessage)};
	template<> MessageData MessageDataEnum<MessageType::REMOVE_SCRIPT			   > = {"REMOVE_SCRIPT", sizeof(RemoveScriptMessage)};
	template<> MessageData MessageDataEnum<MessageType::REMOVE_RIGID_BODY		   > = {"REMOVE_RIGID_BODY", sizeof(RemoveRigidBodyMessage)};
	template<> MessageData MessageDataEnum<MessageType::RENDERER_TOGGLE_CULLING_SPHERE> = {"RENDERER_TOGGLE_CULLING_SPHERE", 0};
	template<> MessageData MessageDataEnum<MessageType::RENDERER_SET_SCALE		   > = {"RENDERER_SET_SCALE", sizeof(RendererSetScaleMessage)};
	template<> MessageData MessageDataEnum<MessageType::EDITOR_TOGGLE		   > = {"EDITOR_TOGGLE", sizeof(RendererSetScaleMessage)};
	template<> MessageData MessageDataEnum<MessageType::COUNT> = {"INVALID", 0};


	const MessageData& getMessageData(MessageType type)
	{
		switch (type)
		{
			TO_SWITCH(SCROLL_UP);
			TO_SWITCH(SCROLL_DOWN);
			TO_SWITCH(KEY_PRESSED);
			TO_SWITCH(KEY_RELEASED);
			TO_SWITCH(BUTTON_PRESSED);
			TO_SWITCH(BUTTON_RELEASED);
			TO_SWITCH(CURSOR_MOVED);
			TO_SWITCH(TOGGLE_CURSOR);
			TO_SWITCH(WINDOW_RESIZED);
			TO_SWITCH(WINDOW_RENAMED);
			TO_SWITCH(PHYSICS_UPDATE_RIGID_BODY);
			TO_SWITCH(PHYSICS_INIT_COLLISION_SHAPE);
			TO_SWITCH(REMOVE_SCRIPT);
			TO_SWITCH(REMOVE_RIGID_BODY);
			TO_SWITCH(RENDERER_TOGGLE_CULLING_SPHERE);
			TO_SWITCH(RENDERER_SET_SCALE);
			TO_SWITCH(EDITOR_TOGGLE);
		}
		EN_ASSERT(false, "Unknown message type: {}", (unsigned int)type);
		return MessageDataEnum<MessageType::COUNT>;
	}

}