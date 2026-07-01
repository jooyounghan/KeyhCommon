#include "CommonBasePch.h"
#include "JsonUtil.h"

namespace keyh
{
    void JsonUtil::TapeElement::setElement(TapeType type, uint64 payload)
    {
        uint64 typeVal = static_cast<uint64>(type);
        _value = (typeVal << 56) | (payload & 0x00FFFFFFFFFFFFFFULL);
    }

    void JsonUtil::TapeElement::setStringElement(uint64 offset, uint32 length)
    {
        uint64 typeVal = static_cast<uint64>(TapeType::String);
        uint64 lengthVal = static_cast<uint64>(length);
        _value = (typeVal << 56) | ((lengthVal & 0xFFFFFF) << 32) | (offset & 0xFFFFFFFF);
    }

    float JsonUtil::TapeElement::parseAsFloat() const
    {
        float value = 0.0f;
        memcpy(&value, &_value, sizeof(float));
        return value;
    }

    int JsonUtil::TapeElement::parseAsInt() const
    {
		return getPayload();
    }

    StringViewA JsonUtil::TapeElement::parseAsStringView(const char* jsonString) const
    {
        return StringViewA(jsonString + getStringOffset(), getStringLength());
    }
}