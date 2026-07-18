#pragma once
#include "ReflectObject.h"
#include "JsonElement.h"
namespace keyh
{
	class IBuffer;
	class JsonElement;

	template<typename T, bool IsReflectObject = IsReflectObject_v<T>>
	struct ReflectSerializer
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonElement& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};

	template<typename T>
	struct ReflectSerializer<T, true>
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonElement& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}
#include "ReflectSerializer.hpp"