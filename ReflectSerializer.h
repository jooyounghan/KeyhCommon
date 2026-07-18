#pragma once
namespace keyh
{
	class IBuffer;
	class JsonElement;

	template<typename T>
	struct ReflectSerializer
	{
		static void serializeToJson(IBuffer* buffer, const T& value);
		static void deserializeFromJson(const JsonElement& json, T& value);
		static void serializeToBinary(IBuffer* buffer, const T& value);
		static void deserializeFromBinary(const void* data, size_t size, T& value);
	};
}
#include "ReflectSerializer.hpp"