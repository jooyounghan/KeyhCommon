namespace keyh
{
    template<typename T, bool IsReflectObject>
    bool ReflectSerializer<T, IsReflectObject>::isEqual(const T& a, const T& b)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectSerializer<T, IsReflectObject>::serializeToJson(IBuffer* buffer, const T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectSerializer<T, IsReflectObject>::deserializeFromJson(const JsonElement& json, T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectSerializer<T, IsReflectObject>::serializeToBinary(IBuffer* buffer, const T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectSerializer<T, IsReflectObject>::deserializeFromBinary(const void* data, size_t size, T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }


#define DECLARE_REFLECT_PROPERTY_SERIALIZER(Type)                                                                   \
    template<> bool ReflectSerializer<Type>::isEqual(const Type& a, const Type& b);                                 \
    template<> void ReflectSerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value);                   \
    template<> void ReflectSerializer<Type>::deserializeFromJson(const JsonElement& json, Type& value);             \
    template<> void ReflectSerializer<Type>::serializeToBinary(IBuffer* buffer, const Type& value);                 \
    template<> void ReflectSerializer<Type>::deserializeFromBinary(const void* data, size_t size, Type& value);

    DECLARE_REFLECT_PROPERTY_SERIALIZER(int8)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int16)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int32)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int64)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint8)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint16)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint32)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint64)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(float)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(double)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(bool)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(StaticStringA)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(FlyweightStringA)
#undef DECLARE_REFLECT_PROPERTY_SERIALIZER

    template <typename T>
    bool ReflectSerializer<T, true>::isEqual(const T& a, const T& b)
    {
        const IReflectObject* reflectObjectA = static_cast<const IReflectObject*>(&a);
        const IReflectObject* reflectObjectB = static_cast<const IReflectObject*>(&b);
        return reflectObjectA->isEqual(reflectObjectB);
    }

    template<typename T>
    void ReflectSerializer<T, true>::serializeToJson(IBuffer* buffer, const T& value)
    {
        constexpr char kObjectBegin = '{';
        constexpr char kObjectEnd = '}';
        constexpr char kDelimiter = ',';
        constexpr char kQuote = '"';
        constexpr char kValueBegin = ':';

        const IReflectObject* reflectObject = static_cast<const IReflectObject*>(&value);

        buffer->writeBytes(&kObjectBegin, 1);

		const ReflectMetaObject& metaObject = reflectObject->getMetaObject();
        const OwnerVector<IReflectProperty>& properties = metaObject.getReflectProperties();

        bool isFirst = true;
		for (const IReflectProperty* property : properties)
		{
			if (property == nullptr)
				continue;

            if (isFirst == false)
				buffer->writeBytes(&kDelimiter, 1);
			
            isFirst = false;
			const FlyweightStringA& propertyName = property->getPropertyName();
			buffer->writeBytes(&kQuote, 1);
			buffer->writeBytes(propertyName.c_str(), propertyName.size());
			buffer->writeBytes(&kQuote, 1);
			buffer->writeBytes(&kValueBegin, 1);
			property->serializeToJson(buffer, reflectObject);
		}

        buffer->writeBytes(&kObjectEnd, 1);
    }

    template<typename T>
    void ReflectSerializer<T, true>::deserializeFromJson(const JsonElement& json, T& value)
    {
        JsonValue jsonValue(json.getContext(json.getIndex()), json.getIndex());
        JsonObject jsonObject = jsonValue.getObjectValue();
        const Vector<Ptr<IReflectProperty>>& properties = value.getMetaObject().getReflectProperties();
        for (JsonKey jsonKey = jsonObject.getFirstKey(); jsonKey.isValid(); jsonKey = jsonObject.getNextKey(jsonKey))
        {
            const StringViewA keyName = jsonKey.getKeyName();
            for (size_t i = 0; i < properties.size(); ++i)
            {
                IReflectProperty* property = properties[i].get();
                if (property == nullptr || property->getPropertyName().getStringView() != keyName)
                    continue;

                property->deserializeFromJson(jsonKey.getValue(), &value);
                break;
            }
        }
    }

    template<typename T>
    void ReflectSerializer<T, true>::serializeToBinary(IBuffer* buffer, const T& value)
    {

    }

    template<typename T>
    void ReflectSerializer<T, true>::deserializeFromBinary(const void* data, size_t size, T& value)
    {

    }

}