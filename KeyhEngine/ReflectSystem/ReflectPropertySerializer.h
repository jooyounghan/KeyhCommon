#pragma once
#include "JsonElement.h"
#include "ReflectionUtil.h"

namespace keyh
{
	class IBuffer;

	template<typename T>
	struct ReflectPropertyPolicy;

	template<typename T, bool IsReflectObject = IsReflectObject_v<T>>
	struct ReflectPropertySerializer
	{
		friend class ReflectPropertyPolicy<T>;

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
		friend class ReflectPropertyPolicy<T>;

	protected:
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}
