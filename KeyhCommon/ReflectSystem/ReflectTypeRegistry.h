#pragma once
#include "IReflectObject.h"

namespace keyh
{
	template<typename ObjectType> class ReflectObject;
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
}
#include "ReflectTypeRegistry.hpp"
