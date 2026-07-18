namespace keyh
{
	template<typename ObjectType, typename ValueType>
	ReflectProperty<ObjectType, ValueType>::ReflectProperty(
		const FlyweightStringA& propertyName
		, const FlyweightStringA& groupName
		, const ValueType& defaultValue
		, ReflectRefGetter<ObjectType, ValueType> refGetter
		, ReflectConstGetter<ObjectType, ValueType> constGetter
	)
		: IReflectProperty(propertyName, groupName), _defaultValue(defaultValue), _refGetter(refGetter), _constGetter(constGetter)
	{
		TypeTrait::requireDerivedFrom<ObjectType, IReflectObject>();
	}

	template<typename ObjectType, typename ValueType>
	ValueType& ReflectProperty<ObjectType, ValueType>::getValueRef(IReflectObject* object) const
	{
		return _refGetter(*static_cast<ObjectType*>(object));
	}

	template<typename ObjectType, typename ValueType>
	const ValueType& ReflectProperty<ObjectType, ValueType>::getValueConstRef(const IReflectObject* object) const
	{
		return _constGetter(*static_cast<const ObjectType*>(object));
	}

	template<typename ObjectType, typename ValueType>
	bool ReflectProperty<ObjectType, ValueType>::isDefault(const IReflectObject* object) const
	{
		return ReflectPropertyPolicy<ValueType>::isEqual(_defaultValue, getValueConstRef(object));
	}

	template<typename ObjectType, typename ValueType>
	bool ReflectProperty<ObjectType, ValueType>::isEqual(const IReflectObject* objectA, const IReflectObject* objectB) const
	{
		return ReflectPropertyPolicy<ValueType>::isEqual(getValueConstRef(objectA), getValueConstRef(objectB));
	}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::serializeToJson(IBuffer* buffer, const IReflectObject* object)
	{
		ReflectPropertyPolicy<ValueType>::serializeToJson(buffer, getValueConstRef(object));
	}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::deserializeFromJson(const JsonValue& jsonValue, IReflectObject* object)
	{
		ReflectPropertyPolicy<ValueType>::deserializeFromJson(jsonValue, getValueRef(object));
	}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::serializeToBinary(IBuffer* buffer, const IReflectObject* object)
	{
		ReflectPropertyPolicy<ValueType>::serializeToBinary(buffer, getValueConstRef(object));
	}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::deserializeFromBinary(const void* data, size_t size, IReflectObject* object)
	{
		ReflectPropertyPolicy<ValueType>::deserializeFromBinary(data, size, getValueRef(object));
	}

}