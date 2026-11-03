namespace keyh
{
	template<typename ObjectType, typename PropertyType>
	void ReflectMetaObject::addReflectProperty(
		const FlyweightStringA& propertyName
		, const FlyweightStringA& groupName
		, ReflectDefaultChecker<PropertyType> defaultChecker
		, ReflectRefGetter<ObjectType, PropertyType> refGetter
		, ReflectConstGetter<ObjectType, PropertyType> constGetter
	)
	{
		const bool isUnique = findProperty(propertyName) == nullptr;
		KEYH_ASSERT(isUnique, "Reflected property names must be unique across the inheritance hierarchy.");
		if (!isUnique)
			return;
		IReflectProperty* property = _properties.emplace_back<ReflectProperty<ObjectType, PropertyType>>(propertyName, groupName, defaultChecker, refGetter, constGetter);
		_propertyMap.insert(propertyName, property);
	}
}
