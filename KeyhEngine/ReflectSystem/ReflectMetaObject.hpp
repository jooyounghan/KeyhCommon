namespace keyh
{
	template<typename ObjectType, typename PropertyType>
	void ReflectMetaObject::addReflectProperty(
		const FlyweightStringA& propertyName
		, const FlyweightStringA& groupName
		, const PropertyType& defaultValue
		, ReflectRefGetter<ObjectType, PropertyType> refGetter
		, ReflectConstGetter<ObjectType, PropertyType> constGetter
	)
	{
		IReflectProperty* property = _properties.emplace_back<ReflectProperty<ObjectType, PropertyType>>(propertyName, groupName, defaultValue, refGetter, constGetter);
		_propertyMap.insert(propertyName, property);
	}
}