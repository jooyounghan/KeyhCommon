#pragma once
#include "JsonElement.h"
#include "ReflectionUtil.h"
#include "IReflectProperty.h"
#include "IReflectObject.h"
#include "ReflectMetaObject.h"
#include "ScalarVectorTypes.h"

namespace keyh
{
	class IBuffer;

	template<typename T>
	struct ReflectPropertyPolicy;

	template<typename EnumType>
	struct ReflectEnumTraits
	{
		static constexpr bool kIsRegistered = false;
		static const char* toString(EnumType) { return nullptr; }
		static bool fromString(const StringViewA&, EnumType&) { return false; }
	};

	// Register Enum <-> string mapping for Reflect JSON serialization.
	// Example:
	// KEYH_REFLECT_ENUM_BEGIN(MyEnum)
	//     KEYH_REFLECT_ENUM_VALUE(MyEnum, ValueA)
	//     KEYH_REFLECT_ENUM_VALUE(MyEnum, ValueB)
	// KEYH_REFLECT_ENUM_END(MyEnum)
#define KEYH_REFLECT_ENUM_BEGIN(EnumType) \
	template<> struct ReflectEnumTraits<EnumType> \
	{ \
		static constexpr bool kIsRegistered = true; \
		static const char* toString(EnumType value) \
		{ \
			switch (value) \
			{

#define KEYH_REFLECT_ENUM_VALUE(EnumType, EnumValue) \
			case EnumType::EnumValue: return #EnumValue;

#define KEYH_REFLECT_ENUM_END(EnumType) \
			default: return nullptr; \
			} \
		} \
		static bool fromString(const StringViewA& name, EnumType& outValue) \
		{

#define KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EnumType, EnumValue) \
			if (name == #EnumValue) { outValue = EnumType::EnumValue; return true; }

#define KEYH_REFLECT_ENUM_END_FROM_STRING() \
			return false; \
		} \
	};

	// -----------------------------------------------------------------------
	// ReflectPropertySerializer
	// Primary template (non-ReflectObject types). Explicit specializations for
	// primitive types are declared below and defined in ReflectSerializer.cpp.
	// -----------------------------------------------------------------------
	template<typename T, bool IsReflectObject = IsReflectObject_v<T>>
	struct ReflectPropertySerializer
	{
		friend struct ReflectPropertyPolicy<T>;

	protected:
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};

	// -----------------------------------------------------------------------
	// ReflectPropertySerializer<T, true>
	// Specialisation for types that derive from IReflectObject.
	// Delegates to the shared helpers on ReflectSerializer.
	// -----------------------------------------------------------------------
	template<typename T>
	struct ReflectPropertySerializer<T, true>
	{
		friend struct ReflectPropertyPolicy<T>;

	protected:
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value, size_t depth = 0, bool pretty = false);
		static void deserializeFromJson(const JsonValue& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};

	// -----------------------------------------------------------------------
	// ReflectSerializer
	// File-level entry points. Also exposes the shared object-level helpers
	// used by ReflectPropertySerializer<T, true>.
	// -----------------------------------------------------------------------
	struct ReflectSerializer
	{
		// File-level API
		static bool serializeToJson(const StringViewA& filePath, const IReflectObject* reflectObject, bool pretty = true);
		static void deserializeFromJson(const StringViewA& filePath, IReflectObject* reflectObject);

		// Shared helpers (used internally and by ReflectPropertySerializer<T,true>)
		static void serializeObjectToBuffer(IBuffer* buffer, const IReflectObject* reflectObject, size_t depth = 0, bool pretty = false);
		static void deserializeObjectFromJson(const JsonObject& jsonObject, IReflectObject* reflectObject);
	};
}

#include "ReflectSerializer.hpp"
