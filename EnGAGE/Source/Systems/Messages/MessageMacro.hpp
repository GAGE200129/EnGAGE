#pragma once

#define EN_MESSAGE_TYPE(x) MessageType getType() const override { return x; }
#define EN_MESSAGE_CATEGORY(x) MessageCategory getCategory() const override { return x; }