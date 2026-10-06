#pragma once
#include "ReflectSerializer.h"
#include "ReflectTypeRegistry.h"

namespace keyh
{
	class IBuffer;

#pragma region Base Policy
	template<typename T>
	struct ReflectPropertyPolicy
	{
		static bool isDefault(const T& value);
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static size_t deserializeFromBinary(const void* data, size_t size, T& value);
	};
#pragma endregion

#pragma region Ptr Policy
	template<typename ElementType>
	struct ReflectPropertyPolicy<Ptr<ElementType>>
	{
		static bool isDefault(const Ptr<ElementType>& value);
		static bool isEqual(const Ptr<ElementType>& a, const Ptr<ElementType>& b);
		static void serializeToJson(IBuffer* buffer, const Ptr<ElementType>& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, Ptr<ElementType>& value);
		static void serializeToBinary(IBuffer* buffer, const Ptr<ElementType>& value);
		static size_t deserializeFromBinary(const void* data, size_t size, Ptr<ElementType>& value);
	};
#pragma endregion

#pragma region Vector Policy
	template<typename ElementType>
	struct ReflectPropertyPolicy<Vector<ElementType>>
	{
		static bool isDefault(const Vector<ElementType>& value);
		static bool isEqual(const Vector<ElementType>& a, const Vector<ElementType>& b);
		static void serializeToJson(IBuffer* buffer, const Vector<ElementType>& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, Vector<ElementType>& value);
		static void serializeToBinary(IBuffer* buffer, const Vector<ElementType>& value);
		static size_t deserializeFromBinary(const void* data, size_t size, Vector<ElementType>& value);
	};
#pragma endregion

#pragma region OwnerVector Policy
	template<typename ElementType>
	struct ReflectPropertyPolicy<OwnerVector<ElementType>>
	{
		static bool isDefault(const OwnerVector<ElementType>& value);
		static bool isEqual(const OwnerVector<ElementType>& a, const OwnerVector<ElementType>& b);
		static void serializeToJson(IBuffer* buffer, const OwnerVector<ElementType>& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, OwnerVector<ElementType>& value);
		static void serializeToBinary(IBuffer* buffer, const OwnerVector<ElementType>& value);
		static size_t deserializeFromBinary(const void* data, size_t size, OwnerVector<ElementType>& value);
	};
#pragma endregion

#pragma region HashMap Policy
	template<typename KeyType, typename ValueType, typename Hasher>
	struct ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>
	{
		static bool isDefault(const HashMap<KeyType, ValueType, Hasher>& value);
		static bool isEqual(const HashMap<KeyType, ValueType, Hasher>& a, const HashMap<KeyType, ValueType, Hasher>& b);
		static void serializeToJson(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, HashMap<KeyType, ValueType, Hasher>& value);
		static void serializeToBinary(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value);
		static size_t deserializeFromBinary(const void* data, size_t size, HashMap<KeyType, ValueType, Hasher>& value);
	};
#pragma endregion
}
#include "ReflectPropertyPolicy.hpp"
