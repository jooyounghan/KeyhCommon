#include "ReflectSystemPch.h"
#include "ReflectSerializer.h"
#include "JsonDocument.h"
#include "FileWriter.h"

namespace keyh
{

// =========================================================================
// ReflectSerializer shared helpers
// =========================================================================

void ReflectSerializer::serializeObjectToBuffer(IBuffer* buffer, const IReflectObject* reflectObject, size_t depth, bool pretty)
{
    buffer->writeBytes(&ReflectionUtil::kObjectBegin, 1);

    const OwnerVector<IReflectProperty>& properties = reflectObject->getReflectProperties();
    bool isFirst = true;
    for (const IReflectProperty* property : properties)
    {
        if (property == nullptr)
            continue;

        if (pretty)
        {
            if (isFirst)
            {
                const char newline = '\n';
                buffer->writeBytes(&newline, 1);
            }
            else
            {
                buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
                const char newline = '\n';
                buffer->writeBytes(&newline, 1);
            }
        }
        else if (isFirst == false)
        {
            buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
        }

        if (pretty)
        {
            for (size_t indent = 0; indent < depth + 1; ++indent)
            {
                buffer->writeBytes("  ", 2);
            }
        }

        const FlyweightStringA& propertyName = property->getPropertyName();
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
        buffer->writeBytes(propertyName.c_str(), propertyName.size());
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
        buffer->writeBytes(&ReflectionUtil::kValueBegin, 1);
        if (pretty)
            buffer->writeBytes(" ", 1);

        property->serializeToJson(buffer, reflectObject, depth + 1, pretty);
        isFirst = false;
    }

    if (pretty && isFirst == false)
    {
        const char newline = '\n';
        buffer->writeBytes(&newline, 1);
        for (size_t indent = 0; indent < depth; ++indent)
        {
            buffer->writeBytes("  ", 2);
        }
    }

    buffer->writeBytes(&ReflectionUtil::kObjectEnd, 1);
}

void ReflectSerializer::deserializeObjectFromJson(const JsonObject& jsonObject, IReflectObject* reflectObject)
{
    for (JsonKey jsonKey = jsonObject.getFirstKey(); jsonKey.isValid(); jsonKey = jsonObject.getNextKey(jsonKey))
    {
        const StringViewA keyName = jsonKey.getKeyName();
        const IReflectProperty* property = reflectObject->findProperty(keyName);
        if (property == nullptr)
            continue;
        property->deserializeFromJson(jsonKey.getValue(), reflectObject);
    }
}

// =========================================================================
// ReflectSerializer file-level API
// =========================================================================

bool ReflectSerializer::serializeToJson(const StringViewA& filePath, const IReflectObject* reflectObject, bool pretty)
{
    if (reflectObject == nullptr)
    {
        KEYH_ASSERT_ARGS(false, "Reflect object is null.");
        return false;
    }

    FileWriter writer;
    if (!writer.open(filePath.c_str()))
    {
        KEYH_ASSERT_ARGS(false, "Failed to open file for writing: %s", filePath.c_str());
        return false;
    }
    serializeObjectToBuffer(&writer, reflectObject, 0, pretty);
    return writer.flush();
}

void ReflectSerializer::deserializeFromJson(const StringViewA& filePath, IReflectObject* reflectObject)
{
    JsonDocument jsonDocument;
    jsonDocument.loadFromFile(filePath.c_str());
    JsonObject rootObject = jsonDocument.getRootObject();

    if (rootObject.isValid() == false)
    {
        KEYH_ASSERT_ARGS(false, "Failed to load JSON document from file: %s", filePath.c_str());
        return;
    }

    deserializeObjectFromJson(rootObject, reflectObject);
}

// =========================================================================
// ReflectPropertySerializer explicit specialisations — isEqual
// =========================================================================
#define DEFINE_IS_EQUAL(Type)                                                                                       \
    template<> bool ReflectPropertySerializer<Type>::isEqual(const Type& a, const Type& b) { return a == b; }
    DEFINE_IS_EQUAL(int8)
    DEFINE_IS_EQUAL(int16)
    DEFINE_IS_EQUAL(int32)
    DEFINE_IS_EQUAL(int64)
    DEFINE_IS_EQUAL(uint8)
    DEFINE_IS_EQUAL(uint16)
    DEFINE_IS_EQUAL(uint32)
    DEFINE_IS_EQUAL(uint64)
    DEFINE_IS_EQUAL(float)
    DEFINE_IS_EQUAL(double)
    DEFINE_IS_EQUAL(bool)
    DEFINE_IS_EQUAL(StaticStringA)
    DEFINE_IS_EQUAL(FlyweightStringA)
#undef DEFINE_IS_EQUAL

// =========================================================================
// ReflectPropertySerializer explicit specialisations — serializeToJson
// =========================================================================
#pragma region serializeToJson

#define DEFINE_SERIALIZE_TO_JSON_SIGNED_INT(Type)                                                                \
    template<> void ReflectPropertySerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value, size_t depth, bool pretty) {            \
        (void)depth;                                                                                             \
        (void)pretty;                                                                                            \
        bool isNegative = static_cast<int64>(value) < 0;                                                        \
        uint64 absValue = isNegative ? static_cast<uint64>(-static_cast<int64>(value)) : static_cast<uint64>(value); \
        StrUtil::intToStr(isNegative, absValue, buffer);                                                        \
    }

#define DEFINE_SERIALIZE_TO_JSON_UNSIGNED_INT(Type)                                                                \
    template<> void ReflectPropertySerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value, size_t depth, bool pretty) {            \
        (void)depth;                                                                                             \
        (void)pretty;                                                                                            \
        StrUtil::intToStr(false, static_cast<uint64>(value), buffer);                                            \
    }

#define DEFINE_SERIALIZE_TO_JSON_FLOAT(Type)                                                                    \
    template<> void ReflectPropertySerializer<Type>::serializeToJson(IBuffer* buffer, const Type& value, size_t depth, bool pretty) {            \
        (void)depth;                                                                                             \
        (void)pretty;                                                                                            \
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
    void ReflectPropertySerializer<bool>::serializeToJson(IBuffer* buffer, const bool& value, size_t depth, bool pretty)
    {
        (void)depth;
        (void)pretty;
        constexpr char kTrue[] = "true";
        constexpr char kFalse[] = "false";
        if (value)
            buffer->writeBytes(kTrue, sizeof(kTrue) - 1);
        else
            buffer->writeBytes(kFalse, sizeof(kFalse) - 1);
    }

    template<>
    void ReflectPropertySerializer<StaticStringA>::serializeToJson(IBuffer* buffer, const StaticStringA& value, size_t depth, bool pretty)
    {
        (void)depth;
        (void)pretty;
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
        buffer->writeBytes(value.c_str(), value.size());
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
    }

    template<>
    void ReflectPropertySerializer<FlyweightStringA>::serializeToJson(IBuffer* buffer, const FlyweightStringA& value, size_t depth, bool pretty)
    {
        (void)depth;
        (void)pretty;
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
        buffer->writeBytes(value.c_str(), value.size());
        buffer->writeBytes(&ReflectionUtil::kQuote, 1);
    }

#pragma endregion

// =========================================================================
// ReflectPropertySerializer explicit specialisations — deserializeFromJson
// =========================================================================
#pragma region deserializeFromJson

#define DEFINE_DESERIALIZE_FROM_JSON_INT(Type)                                                          \
    template<> void ReflectPropertySerializer<Type>::deserializeFromJson(const JsonValue& json, Type& value)    \
    {                                                                                                   \
        value = static_cast<Type>(json.getIntValue());                                                   \
    }

#define DEFINE_DESERIALIZE_FROM_JSON_FLOAT(Type)                                                        \
    template<> void ReflectPropertySerializer<Type>::deserializeFromJson(const JsonValue& json, Type& value)    \
    {                                                                                                   \
        value = static_cast<Type>(json.getFloatValue());                                                 \
    }

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
    void ReflectPropertySerializer<bool>::deserializeFromJson(const JsonValue& json, bool& value)
    {
        value = json.getBoolValue();
    }

    template<>
    void ReflectPropertySerializer<StaticStringA>::deserializeFromJson(const JsonValue& json, StaticStringA& value)
    {
        StringViewA stringView = json.getStringValue();
        value = StaticStringA(stringView.c_str(), stringView.length());
    }

    template<>
    void ReflectPropertySerializer<FlyweightStringA>::deserializeFromJson(const JsonValue& json, FlyweightStringA& value)
    {
        StringViewA stringView = json.getStringValue();
        value = FlyweightStringA(stringView);
    }

#pragma endregion

// =========================================================================
// ReflectPropertySerializer explicit specialisations — serializeToBinary
// =========================================================================
#pragma region serializeToBinary
    // TODO: Implement binary serialization for each type in a future step.

#define DEFINE_SERIALIZE_TO_BINARY(Type)                                                                        \
    template<> void ReflectPropertySerializer<Type>::serializeToBinary(IBuffer* buffer, const Type& value) {}

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

// =========================================================================
// ReflectPropertySerializer explicit specialisations — deserializeFromBinary
// =========================================================================
#pragma region deserializeFromBinary
    // TODO: Implement binary deserialization for each type in a future step.

#define DEFINE_DESERIALIZE_FROM_BINARY(Type)                                                                                        \
    template<> void ReflectPropertySerializer<Type>::deserializeFromBinary(const void* data, size_t size, Type& value) {}

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
