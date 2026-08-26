namespace keyh
{
    // -----------------------------------------------------------------------
    // Primary template stubs — all primitive specialisations are in the .cpp.
    // -----------------------------------------------------------------------
    template<typename T, bool IsReflectObject>
    bool ReflectPropertySerializer<T, IsReflectObject>::isEqual(const T& a, const T& b)
    {
        if constexpr (IsEnum_v<T>)
        {
            return a == b;
        }
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::serializeToJson(IBuffer* buffer, const T& value, size_t depth, bool pretty)
    {
        if constexpr (IsEnum_v<T>)
        {
            (void)depth;
            (void)pretty;

            if constexpr (ReflectEnumTraits<T>::kIsRegistered)
            {
                const char* enumName = ReflectEnumTraits<T>::toString(value);
                if (enumName != nullptr)
                {
                    buffer->writeBytes(&ReflectionUtil::kQuote, 1);
                    buffer->writeBytes(enumName, StrUtil::strlen(enumName));
                    buffer->writeBytes(&ReflectionUtil::kQuote, 1);
                    return;
                }
            }

            KEYH_ASSERT_ARGS(false, "Unregistered or unknown enum value for JSON serialization.");
            const bool isNegative = static_cast<int64>(value) < 0;
            const uint64 absValue = isNegative
                ? static_cast<uint64>(-static_cast<int64>(value))
                : static_cast<uint64>(value);
            StrUtil::intToStr(isNegative, absValue, buffer);
            return;
        }
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::deserializeFromJson(const JsonValue& json, T& value)
    {
        if constexpr (IsEnum_v<T>)
        {
            if (json.getValueType() == JsonUtil::TapeType::String)
            {
                if constexpr (ReflectEnumTraits<T>::kIsRegistered)
                {
                    if (ReflectEnumTraits<T>::fromString(json.getStringValue(), value))
                        return;
                }

                KEYH_ASSERT_ARGS(false, "Invalid enum string while deserializing JSON.");
                return;
            }

            value = static_cast<T>(json.getIntValue());
            return;
        }
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::serializeToBinary(IBuffer* buffer, const T& value)
    {
        if constexpr (IsEnum_v<T>)
        {
            (void)buffer;
            (void)value;
            return;
        }
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }

    template<typename T, bool IsReflectObject>
    void ReflectPropertySerializer<T, IsReflectObject>::deserializeFromBinary(const void* data, size_t size, T& value)
    {
        if constexpr (IsEnum_v<T>)
        {
            (void)data;
            (void)size;
            (void)value;
            return;
        }
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
    }


#define DECLARE_REFLECT_PROPERTY_SERIALIZER(Type)                                                                   \
    template<> bool ReflectPropertySerializer<Type>::isEqual(const Type& a, const Type& b);                                 \
    template<> void ReflectPropertySerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value, size_t depth, bool pretty);                   \
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
    DECLARE_REFLECT_PROPERTY_SERIALIZER(float2)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(float3)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(float4)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int2)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int3)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(int4)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint2)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint3)
    DECLARE_REFLECT_PROPERTY_SERIALIZER(uint4)
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
    void ReflectPropertySerializer<T, true>::serializeToJson(IBuffer* buffer, const T& value, size_t depth, bool pretty)
    {
        const IReflectObject* reflectObject = static_cast<const IReflectObject*>(&value);
        ReflectSerializer::serializeObjectToBuffer(buffer, reflectObject, depth, pretty);
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
