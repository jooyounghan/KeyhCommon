#pragma once
#include "ReflectionUtil.h"
#include "ReflectProperty.h"

namespace keyh
{
	class IReflectProperty;

	class ReflectMetaObject
	{
	public:
		ReflectMetaObject() = default;
		
	protected:
		OwnerVector<IReflectProperty>					_properties;
		HashMap<FlyweightStringA, IReflectProperty*>	_propertyMap;

	public:
		IReflectProperty* findProperty(const FlyweightStringA& propertyName);

	public:
		template<typename ObjectType, typename PropertyType>
		void addReflectProperty(
			const FlyweightStringA& propertyName
			, const FlyweightStringA& groupName
			, const PropertyType& defaultValue
			, ReflectRefGetter<ObjectType, PropertyType> refGetter
			, ReflectConstGetter<ObjectType, PropertyType> constGetter
		);
	};
}
#include "ReflectMetaObject.hpp"