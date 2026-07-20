namespace keyh
{
    // -----------------------------------------------------------------------
    // Primary template stubs — all primitive specialisations are in the .cpp.
    // -----------------------------------------------------------------------
    template<typename T, bool IsReflectObject>
    bool ReflectPropertySerializer<T, IsReflectObject>::isEqual(const T& a, const T& b)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::serializeToJson(IBuffer* buffer, const T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::deserializeFromJson(const JsonValue& json, T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::serializeToBinary(IBuffer* buffer, const T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::deserializeFromBinary(const void* data, size_t size, T& value)
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }


#define DECLARE_REFLECT_PROPERTY_SERIALIZER(Type)                                                                   \
    template<> bool ReflectPropertySerializer<Type>::isEqual(const Type& a, const Type& b);                                 \
    template<> void ReflectPropertySerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value);                   \
    template<> void ReflectPropertySerializer<Type>::deserializeFromJson(const JsonValue& json, Type& value);             \
    template<> void ReflectPropertySerializer<Type>::serializeToBinary(IBuffer* buffer, const Type& value);                 \
    template<> void ReflectPropertySerializer<Type>::deserializeFromBinary(const void* data, size_t size, Type& value);

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

    // -----------------------------------------------------------------------
    // ReflectPropertySerializer<T, true> — IReflectObject-derived types.
    // Delegates all object-level work to the shared helpers on ReflectSerializer.
    // -----------------------------------------------------------------------
    template <typename T>
    bool ReflectPropertySerializer<T, true>::isEqual(const T& a, const T& b)
    {
        const IReflectObject* reflectObjectA = static_cast<const IReflectObject*>(&a);
        const IReflectObject* reflectObjectB = static_cast<const IReflectObject*>(&b);
        return reflectObjectA->isEqual(reflectObjectB);
    }

    template<typename T>
    void ReflectPropertySerializer<T, true>::serializeToJson(IBuffer* buffer, const T& value)
    {
        const IReflectObject* reflectObject = static_cast<const IReflectObject*>(&value);
        ReflectSerializer::serializeObjectToBuffer(buffer, reflectObject);
    }

    template<typename T>
    void ReflectPropertySerializer<T, true>::deserializeFromJson(const JsonValue& json, T& value)
    {
        JsonObject jsonObject = json.getObjectValue();
        if (jsonObject.isValid() == false)
        {
            KEYH_ASSERT(false, "JSON value is not an object.");
            return;
        }
        ReflectSerializer::deserializeObjectFromJson(jsonObject, &value);
    }

    template<typename T>
    void ReflectPropertySerializer<T, true>::serializeToBinary(IBuffer* buffer, const T& value)
    {
    }

    template<typename T>
    void ReflectPropertySerializer<T, true>::deserializeFromBinary(const void* data, size_t size, T& value)
    {
    }

}
