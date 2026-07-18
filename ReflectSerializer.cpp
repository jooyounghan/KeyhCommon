#include "ReflectSystemPch.h"
#include "ReflectSerializer.h"
#include "JsonElement.h"

namespace keyh
{

#pragma region serializeToJson

#define DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(Type)                                                                \
    template<> void ReflectSerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value) {            \
        bool isNegative = static_cast<int64>(value) < 0;                                                        \
        uint64 absValue = isNegative ? static_cast<uint64>(-static_cast<int64>(value)) : static_cast<uint64>(value); \
        StrUtil::intToStr(isNegative, absValue, buffer);                                                        \
    }

#define DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(Type)                                                                \
    template<> void ReflectSerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value) {            \
        StrUtil::intToStr(false, static_cast<uint64>(value), buffer);                                            \
    }

#define DEFINE_SERIALIZE_TO_JSON_FLOAT(Type)                                                                    \
    template<> void ReflectSerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value) {            \
        StrUtil::floatToStr(static_cast<double>(value), buffer);                                                \
    }

    DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(int8)
        DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(int16)
        DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(int32)
        DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(int64)
        DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(uint8)
        DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(uint16)
        DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(uint32)
        DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(uint64)

        DEFINE_SERIALIZE_TO_JSON_FLOAT(float)
        DEFINE_SERIALIZE_TO_JSON_FLOAT(double)

#undef DEFINE_SERIALIZE_TO_JSON_SIGNED_INT
#undef DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT
#undef DEFINE_SERIALIZE_TO_JSON_FLOAT

        template<>
    void ReflectSerializer<bool>::serializeToJson(IBuffer* buffer, const bool& value)
    {
        constexpr char kTrue[] = "true";
        constexpr char kFalse[] = "false";
        if (value)
            buffer->writeBytes(kTrue, sizeof(kTrue) - 1);
        else
            buffer->writeBytes(kFalse, sizeof(kFalse) - 1);
    }

    template<>
    void ReflectSerializer<StaticStringA>::serializeToJson(IBuffer* buffer, const StaticStringA& value)
    {
        buffer->writeBytes(value.c_str(), value.size());
    }

    template<>
    void ReflectSerializer<FlyweightStringA>::serializeToJson(IBuffer* buffer, const FlyweightStringA& value)
    {
        buffer->writeBytes(value.c_str(), value.size());
    }

#pragma endregion

#pragma region deserializeFromJson
    // TODO: Implement JSON deserialization for each type in a future step.
    //       These stubs exist so that ReflectSerializer<T> is fully specialized
    //       for all built-in types even before deserialization logic is written.

#define DEFINE_DESERIALIZE_FROM_JSON_INT(Type)                                                                        \
    template<> void ReflectSerializer<Type>::deserializeFromJson(const JsonElement& json, Type& value) {}

#define DEFINE_DESERIALIZE_FROM_JSON_FLOAT(Type)                                                                    \
    template<> void ReflectSerializer<Type>::deserializeFromJson(const JsonElement& json, Type& value) {}

    DEFINE_DESERIALIZE_FROM_JSON_INT(int8)
        DEFINE_DESERIALIZE_FROM_JSON_INT(int16)
        DEFINE_DESERIALIZE_FROM_JSON_INT(int32)
        DEFINE_DESERIALIZE_FROM_JSON_INT(int64)
        DEFINE_DESERIALIZE_FROM_JSON_INT(uint8)
        DEFINE_DESERIALIZE_FROM_JSON_INT(uint16)
        DEFINE_DESERIALIZE_FROM_JSON_INT(uint32)
        DEFINE_DESERIALIZE_FROM_JSON_INT(uint64)

        DEFINE_DESERIALIZE_FROM_JSON_FLOAT(float)
        DEFINE_DESERIALIZE_FROM_JSON_FLOAT(double)

#undef DEFINE_DESERIALIZE_FROM_JSON_INT
#undef DEFINE_DESERIALIZE_FROM_JSON_FLOAT

        template<>
    void ReflectSerializer<bool>::deserializeFromJson(const JsonElement& json, bool& value) {}

    template<>
    void ReflectSerializer<StaticStringA>::deserializeFromJson(const JsonElement& json, StaticStringA& value) {}

    template<>
    void ReflectSerializer<FlyweightStringA>::deserializeFromJson(const JsonElement& json, FlyweightStringA& value) {}

#pragma endregion

#pragma region serializeToBinary
    // TODO: Implement binary serialization for each type in a future step.

#define DEFINE_SERIALIZE_TO_BINARY(Type)                                                                        \
    template<> void ReflectSerializer<Type>::serializeToBinary(IBuffer* buffer, const Type& value) {}

    DEFINE_SERIALIZE_TO_BINARY(int8)
        DEFINE_SERIALIZE_TO_BINARY(int16)
        DEFINE_SERIALIZE_TO_BINARY(int32)
        DEFINE_SERIALIZE_TO_BINARY(int64)
        DEFINE_SERIALIZE_TO_BINARY(uint8)
        DEFINE_SERIALIZE_TO_BINARY(uint16)
        DEFINE_SERIALIZE_TO_BINARY(uint32)
        DEFINE_SERIALIZE_TO_BINARY(uint64)
        DEFINE_SERIALIZE_TO_BINARY(float)
        DEFINE_SERIALIZE_TO_BINARY(double)
        DEFINE_SERIALIZE_TO_BINARY(bool)
        DEFINE_SERIALIZE_TO_BINARY(StaticStringA)
        DEFINE_SERIALIZE_TO_BINARY(FlyweightStringA)

#undef DEFINE_SERIALIZE_TO_BINARY

#pragma endregion

#pragma region deserializeFromBinary
        // TODO: Implement binary deserialization for each type in a future step.

#define DEFINE_DESERIALIZE_FROM_BINARY(Type)                                                                                        \
    template<> void ReflectSerializer<Type>::deserializeFromBinary(const void* data, size_t size, Type& value) {}

        DEFINE_DESERIALIZE_FROM_BINARY(int8)
        DEFINE_DESERIALIZE_FROM_BINARY(int16)
        DEFINE_DESERIALIZE_FROM_BINARY(int32)
        DEFINE_DESERIALIZE_FROM_BINARY(int64)
        DEFINE_DESERIALIZE_FROM_BINARY(uint8)
        DEFINE_DESERIALIZE_FROM_BINARY(uint16)
        DEFINE_DESERIALIZE_FROM_BINARY(uint32)
        DEFINE_DESERIALIZE_FROM_BINARY(uint64)
        DEFINE_DESERIALIZE_FROM_BINARY(float)
        DEFINE_DESERIALIZE_FROM_BINARY(double)
        DEFINE_DESERIALIZE_FROM_BINARY(bool)
        DEFINE_DESERIALIZE_FROM_BINARY(StaticStringA)
        DEFINE_DESERIALIZE_FROM_BINARY(FlyweightStringA)

#undef DEFINE_DESERIALIZE_FROM_BINARY

#pragma endregion

}