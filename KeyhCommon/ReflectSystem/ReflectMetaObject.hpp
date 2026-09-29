namespace keyh
{
	inline ReflectMetaObject ReflectMetaObject::clone() const
	{
		ReflectMetaObject result;
		for (const IReflectProperty* property : _properties)
		{
			KEYH_ASSERT(property != nullptr, "Reflected metadata cannot contain a null property.");
			if (property == nullptr)
				continue;
			IReflectProperty* copy = result._properties.push_back(property->clone());
			result._propertyMap.insert(copy->getPropertyName(), copy);
		}
		return result;
	}

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
