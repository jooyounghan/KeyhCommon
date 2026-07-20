#pragma once
#include "JsonElement.h"
#include "ReflectionUtil.h"
#include "IReflectProperty.h"
#include "IReflectObject.h"

namespace keyh
{
	class IBuffer;

	template<typename T>
	struct ReflectPropertyPolicy;

	template<typename T, bool IsReflectObject = IsReflectObject_v<T>>
	struct ReflectPropertySerializer
	{
		friend struct ReflectPropertyPolicy<T>;

	protected:
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};

	template<typename T>
	struct ReflectPropertySerializer<T, true>
	{
		friend struct ReflectPropertyPolicy<T>;

	protected:
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}

#include "ReflectPropertySerializer.hpp"
