#pragma once
namespace keyh
{
	class ReflectSerializer
	{
	public:
		template<typename T>
		static void serializeToJson(const T& object);
	};
}
#include "ReflectSerializer.hpp"