#pragma once
#include "IReflectObject.h"

namespace keyh
{
	template<typename ObjectType> class ReflectObject;
	template<typename ObjectType>
	struct ReflectParentType
	{
		using Type = void;
	};
	// Register stable wire names during startup, before concurrent serialization.
	// A separate registry per base type makes every factory result type-safe.
	template<typename BaseType>
	class ReflectTypeRegistry
	{
		struct Entry
		{
			FlyweightStringA name;
			const ReflectMetaObject* metaObject;
			Ptr<BaseType> (*create)();
		};
		static Vector<Entry>& entries();

	public:
		template<typename DerivedType>
		static bool registerType(const StringViewA& name);
		static StringViewA findName(const BaseType& object);
		static Ptr<BaseType> create(const StringViewA& name);
		static Ptr<BaseType> createLegacy();
	};

	template<typename DerivedType, typename BaseType>
	struct ReflectAncestorRegistrar
	{
		static bool registerType(const StringViewA& name)
		{
			if (!ReflectTypeRegistry<BaseType>::template registerType<DerivedType>(name))
				return false;
			using ParentType = typename ReflectParentType<BaseType>::Type;
			return ReflectAncestorRegistrar<DerivedType, ParentType>::registerType(name);
		}
	};

	template<typename DerivedType>
	struct ReflectAncestorRegistrar<DerivedType, void>
	{
		static bool registerType(const StringViewA&) { return true; }
	};
}
#include "ReflectTypeRegistry.hpp"
