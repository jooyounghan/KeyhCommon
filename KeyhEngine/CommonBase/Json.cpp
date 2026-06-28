#include "CommonBasePch.h"
#include "Json.h"

namespace keyh
{
    void JsonDocument::TapeElement::setElement(TapeType type, uint64_t payload)
    {
        uint64_t typeVal = static_cast<uint64_t>(type);
        _value = (typeVal << 56) | (payload & 0x00FFFFFFFFFFFFFFULL);
    }
    
    void JsonDocument::TapeElement::setStringElement(uint64_t offset, uint32_t length)
    {
        uint64_t typeVal = static_cast<uint64_t>(TapeType::String);
        uint64_t lengthVal = static_cast<uint64_t>(length);
        _value = (typeVal << 56) | ((lengthVal & 0xFFFFFF) << 32) | (offset & 0xFFFFFFFF);
    }
}