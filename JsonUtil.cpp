#include "CommonBasePch.h"
#include "JsonUtil.h"

namespace keyh
{
    void JsonUtil::TapeElement::setIndexElement(TapeType type, size_t index)
    {
        const uint64 typeVal = static_cast<uint64>(type);
		const uint64 indexVal = static_cast<uint64>(index);
        _value = (typeVal << kTypeShift) | (indexVal & kIndexMask);
    }

    void JsonUtil::TapeElement::setStringElement(size_t offset, size_t length)
    {
        const uint64 typeVal = static_cast<uint64>(TapeType::String);
        const uint64 lengthVal = static_cast<uint64>(length);
        _value = (typeVal << kTypeShift) | ((lengthVal & kStringLengthMask) << kStringLengthShift) | (offset & kStringOffsetMask);
    }

    template<typename T>
    T JsonUtil::TapeElement::parse() const
    {
        STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(JsonUtil::TapeElement<T>);
    }

	StringViewA JsonUtil::TapeElement::parse(const char* jsonString) const
	{
        return StringViewA(jsonString + getPayloadAsStringOffset(), getPayloadAsStringLength());
	}

    template<typename T>
    T JsonUtil::TapeElement::parseImpl() const
    {
        static_assert(sizeof(T) <= 7, "Type T must be 8 bytes or smaller.");
        constexpr uint64_t mask = kInvalidUint64 >> (64 - (sizeof(T) * 8));
        uint64_t result = _value & mask;

        T value;
        memcpy(&value, &result, sizeof(T));
        return value;
    }
}