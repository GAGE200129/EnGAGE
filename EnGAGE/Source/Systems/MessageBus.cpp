#include "pch.hpp"
#include "MessageBus.hpp"

namespace Systems {
    std::shared_ptr<Messaging::Message> MessageBus::queryMessage()
    {
        auto message = mMessages.front();
        mMessages.pop();
        return message;
    }
    void MessageBus::receiveMessage(const std::shared_ptr<Messaging::Message>& message)
    {
        mMessages.push(message);
    }
    bool MessageBus::empty() const
    {
        return mMessages.empty();
    }
}
