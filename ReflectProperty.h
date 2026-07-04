#pragma once
#include "IReflectProperty.h"

namespace keyh
{
	template<typename ObjectType, typename ValueType>
	class ReflectProperty : public ReflectPropertyBase<ObjectType, ValueType>
	{
	public:
		using RefGetter = ReflectPropertyBase<ObjectType, ValueType>::RefGetter;
		using ConstGetter = ReflectPropertyBase<ObjectType, ValueType>::ConstGetter;

		ReflectProperty(const FlyweightStringA& name, RefGetter refGetter, ConstGetter constGetter)
			: ReflectPropertyBase<ObjectType, ValueType>(name, refGetter, constGetter)
		{
		}


	};
}

