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
		Ptr<IReflectProperty> property = makePtr<ReflectProperty<ObjectType, PropertyType>>(propertyName, groupName, defaultValue, refGetter, constGetter);
		_properties.push_back(keyh::move(property));
		_propertyMap.insert(propertyName, property.get());
	}
}