#pragma once
#include "FlyweightString.h"
#include "JsonElement.h"

namespace keyh
{
	class IReflectProperty
	{
	public:
		virtual ~IReflectProperty() = default;

	protected:
		FlyweightStringA _name;

	public:
		inline const FlyweightStringA& getName() const { return _name; }

	public:
		void serailizeToJson();
		void deserializeFromJson(const JsonElement& jsonElement);
		void serializeToBinary();
		void deserializeFromBinary(const void* data);
	};
}
