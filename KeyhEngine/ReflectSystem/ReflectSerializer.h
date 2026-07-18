#pragma once
#include "ReflectionUtil.h"
#include "IReflectObject.h"
#include "ReflectMetaObject.h"
#include "IReflectProperty.h"
#include "JsonElement.h"

namespace keyh
{
	class IBuffer;
	class JsonValue;

	template<typename T, bool IsReflectObject = IsReflectObject_v<T>>
	struct ReflectSerializer
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};

	template<typename T>
	struct ReflectSerializer<T, true>
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}

#include "ReflectSerializer.hpp"