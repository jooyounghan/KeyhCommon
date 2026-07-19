#pragma once
#include "ReflectPropertySerializer.h"
#include "ReflectMetaObject.h"

namespace keyh
{
	class IBuffer;

	struct ReflectSerializer
	{
		static void serializeToJson(const StringViewA& filePath, const IReflectObject* reflectObject);
		static void deserializeFromJson(const StringViewA& filePath, IReflectObject* reflectObject);
	};
}