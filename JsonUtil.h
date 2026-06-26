#pragma once
#include "CommonCore.h"

namespace keyh
{
	struct JsonUtil
	{
		static constexpr const char* kNull = "null";
		static constexpr const char* kTrue = "true";
		static constexpr const char* kFalse = "false";
	};

    enum class TapeType : uint64
    {
        ObjectStart = '{',
        ObjectEnd = '}',
        String = '"',
        Integer = 'l'
    };

    struct TapeElement
    {
        uint64 _value;

        void setElement(TapeType type, uint64 payload)
        {
            uint64 typeVal = static_cast<uint64>(type);
            _value = (typeVal << 56) | (payload & 0x00FFFFFFFFFFFFFFULL);
        }

        TapeType getType() const
        {
            return static_cast<TapeType>(_value >> 56);
        }

        uint64 getPayload() const
        {
            return _value & 0x00FFFFFFFFFFFFFFULL;
        }
    };
}