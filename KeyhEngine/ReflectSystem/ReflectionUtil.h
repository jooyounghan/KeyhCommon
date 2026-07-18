#pragma once
#include "Vector.h"
#include "HashMap.h"
#include "HashSet.h"

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

#pragma region Reflect Type Trait
	template <typename T>
	struct IsVectorImpl
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct IsVectorImpl<Vector<T>>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsVector_v = IsVectorImpl<Decay_t<T>>::value;

	template<typename T>
	struct IsHashMapImpl
	{
		static constexpr bool value = false;
	};

	template<typename KeyType, typename ValueType, typename Hasher>
	struct IsHashMapImpl<HashMap<KeyType, ValueType, Hasher>>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsHashMap_v = IsHashMapImpl<Decay_t<T>>::value;

	template<typename T>
	struct IsHashSetImpl
	{
		static constexpr bool value = false;
	};

	template<typename KeyType, typename Hasher>
	struct IsHashSetImpl<HashSet<KeyType, Hasher>>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsHashSet_v = IsHashSetImpl<Decay_t<T>>::value;
#pragma endregion
}