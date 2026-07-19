#pragma once
#include "JsonElement.h"
#include "IReflectObject.h"
#include "ReflectMetaObject.h"

namespace keyh
{
	class IBuffer;

	struct ReflectSerializer
	{
		static void serializeToJson(const StringViewA& filePath, const IReflectObject* reflectObject);
		static void deserializeFromJson(const StringViewA& filePath, IReflectObject* reflectObject);
	};

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

#include "ReflectSerializer.hpp"