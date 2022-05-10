#pragma once

#include <cstdint>
#include <queue>
#include <memory>
#include <string>
#include <cassert>
#include <iostream>
#include <bitset>
#include <array>
#include <unordered_map>
#include <set>
#include <fstream>
#include <map>
#include <filesystem>
#include <bitset>
#include <random>
#include <sstream>
#include <regex>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "Core/Log.hpp"

#ifdef EN_DEBUG
#define EN_ASSERT(condition, ...)\
		if(!(condition)) {\
			EN_ERROR("Assertion failed ({}), function: {}, line: {}, file: {}", #condition, __FUNCTION__, __FILE__, __LINE__);\
			EN_ERROR(__VA_ARGS__);\
			abort();\
		}
#else
#define EN_ASSERT(condition, ...) condition
#endif

//Data type name

using UInt8 = uint8_t;
using UInt16 = uint16_t;
using UInt32 = uint32_t;
using UInt64 = uint64_t;
using Byte = int8_t;
using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;
using F32 = float;
using F64 = double;
using CStr = const char*;
using Vec3 = glm::vec3;
using Mat4x4 = glm::mat4x4;


using Thread = std::thread;

template<typename T>
using FunctionPtr = std::function<T>;

template<typename K, typename V>
using UnorderedMap = std::unordered_map<K, V>;

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename T>
using DynArr = std::vector<T>;

template<typename T, size_t Count>
using Arr = std::array<T, Count>;

using String = std::string;

template<typename T>
using Queue = std::queue<T>;

template<typename T>
using Set = std::set<T>;

template<size_t Bits>
using BitSet = std::bitset<Bits>;

//Alias for weak_ptr
template<typename T>
using Ptr = std::weak_ptr<T>;

//Alias for shared_ptr
template<typename T>
using Ref = std::shared_ptr<T>;

//Alias for unique_ptr
template<typename T>
using Scope = std::unique_ptr<T>;


template<typename T, typename... Args>
Ref<T> createRef(Args&&... args)
{
	return std::make_shared<T>(args...);
}

template<typename T, typename... Args>
Scope<T> createScope(Args&&... args)
{
	return std::make_unique<T>(args...);
}

template<typename To, typename From>
Ref<To> staticCast(const Ref<From>& from)
{
	return std::static_pointer_cast<To>(from);
}