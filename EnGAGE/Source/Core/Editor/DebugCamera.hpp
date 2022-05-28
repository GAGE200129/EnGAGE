#pragma once

#include "Core/Messenger/Messenger.hpp"

namespace Core::DebugCamera
{
	void onMessage(const Message* pMessage);
	void update(F32 delta);
}