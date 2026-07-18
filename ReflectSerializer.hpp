namespace keyh
{
#define DECLARE_REFLECT_PROPERTY_SERIALIZER(Type)                                                                   \
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

#undef DECLARE_REFLECT_PROPERTY_POLICY
}