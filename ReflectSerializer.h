#pragma once
namespace keyh
{
	class IBuffer;

	class ReflectSerializer
	{
	public:
		template<typename T>
		static void serializeToBuffer(const T& object, IBuffer* buffer);

		template<typename T>
		static void deserializeFromBuffer(const void* buffer, T& object);
	};
}
#include "ReflectSerializer.hpp"