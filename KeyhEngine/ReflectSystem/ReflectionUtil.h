#pragma once

namespace keyh
{
	// Plain function-pointer types used for reflection property accessors.
	// Stateless lambdas (no captures) are implicitly convertible to these types,
	// so generated initializeMetaObject() code can pass lambdas directly.
	// Example:
	//   [](MyClass& obj) -> int&             { return obj._value; }   // ReflectRefGetter
	//   [](const MyClass& obj) -> const int& { return obj._value; }   // ReflectConstGetter

	template<typename ObjectType, typename PropertyType>
	using ReflectRefGetter = PropertyType& (*)(ObjectType&);

	template<typename ObjectType, typename PropertyType>
	using ReflectConstGetter = const PropertyType& (*)(const ObjectType&);
}