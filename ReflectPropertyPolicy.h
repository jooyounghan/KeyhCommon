#pragma once
#include "FlyweightString.h"
#include "StaticString.h"

namespace keyh
{
	class IBuffer;
	class JsonElement;

	// Policy-based trait controlling how a reflected property of type T is
	// compared and serialized.  Specialize this struct for any type that you
	// want to use as a reflected property.
	//
	// Required specialization interface:
	//   static bool isEqual(const T& a, const T& b);
	//   static void serializeToJson(IBuffer* buffer, const T& value);
	//   static void deserializeFromJson(const JsonElement& json, T& value);
	//   static void serializeToBinary(IBuffer* buffer, const T& value);
	//   static void deserializeFromBinary(const void* data, size_t size, T& value);
	//
	// The primary template triggers a static_assert for serialization methods so
	// that missing specializations produce a clear compile-time error.
	// isEqual falls back to operator== by default, which is sufficient for all
	// arithmetic and string types built into this engine.
	template<typename T>
	struct ReflectPropertyPolicy
	{
		static bool isEqual(const T& a, const T& b);
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonElement& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}
#include "ReflectPropertyPolicy.hpp"
