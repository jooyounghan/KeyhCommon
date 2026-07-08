#pragma once

namespace keyh
{
	template<typename ObjectType, typename PropertyType>
	using ReflectRefGetter = PropertyType & (ObjectType::*)();

	template<typename ObjectType, typename PropertyType>
	using ReflectConstGetter = const PropertyType& (ObjectType::*)();
}