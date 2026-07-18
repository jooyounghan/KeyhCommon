#pragma once
#include "ReflectionUtil.h"
#include "ReflectSerializer.h"

namespace keyh
{
	class IBuffer;
	class JsonElement;

#pragma region Base Policy
	template<typename T>
	struct ReflectPropertyPolicy
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonElement& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
#pragma endregion

#pragma region Vector Policy
	template<typename ElementType>
	struct ReflectPropertyPolicy<Vector<ElementType>>
	{
		static bool isEqual(const Vector<ElementType>& a, const Vector<ElementType>& b);
		static void serializeToJson(IBuffer* buffer, const Vector<ElementType>& value);
		static void deserializeFromJson(const JsonElement& json, Vector<ElementType>& value);
		static void serializeToBinary(IBuffer* buffer, const Vector<ElementType>& value);
		static void deserializeFromBinary(const void* data, size_t size, Vector<ElementType>& value);
	};
#pragma endregion

#pragma region HashMap Policy
	template<typename KeyType, typename ValueType, typename Hasher>
	struct ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>
	{
		static bool isEqual(const HashMap<KeyType, ValueType, Hasher>& a, const HashMap<KeyType, ValueType, Hasher>& b);
		static void serializeToJson(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value);
		static void deserializeFromJson(const JsonElement& json, HashMap<KeyType, ValueType, Hasher>& value);
		static void serializeToBinary(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value);
		static void deserializeFromBinary(const void* data, size_t size, HashMap<KeyType, ValueType, Hasher>& value);
	};
#pragma endregion
}
#include "ReflectPropertyPolicy.hpp"
