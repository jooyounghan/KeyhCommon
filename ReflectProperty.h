#pragma once
#include "IReflectProperty.h"

namespace keyh
{
	template<typename ObjectType, typename ValueType>
	class ReflectProperty : public ReflectPropertyBase<ObjectType, ValueType>
	{
	public:
		using Base = ReflectPropertyBase<ObjectType, ValueType>;
		using RefGetter = Base::RefGetter;
		using ConstGetter = Base::ConstGetter;
		using Base::_refGetter;
		using Base::_constGetter;

		ReflectProperty(const FlyweightStringA& name, RefGetter refGetter, ConstGetter constGetter)
			: ReflectPropertyBase<ObjectType, ValueType>(name, refGetter, constGetter)
		{
		}

	public:
		virtual void serializeToBuffer(IBuffer* buffer) override;
		virtual void deserializeFromJson(const JsonElement& jsonElement) override;
		virtual void serializeToBinary(IBuffer* buffer) override;
		virtual void deserializeFromBinary(const void* data, size_t size) override;
	};
}
#include "ReflectProperty.hpp"

