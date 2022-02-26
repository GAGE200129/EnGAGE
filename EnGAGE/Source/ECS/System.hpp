#pragma once

#include "pch.hpp"

#include "ECS.hpp"

#include "Systems/Messages/Message.hpp"

namespace ECS
{
	class System
	{
	public:
		virtual void handleMessage(std::shared_ptr<Messaging::Message> message) = 0;
	public:
		std::set<Entity> mEntities;
	};
}