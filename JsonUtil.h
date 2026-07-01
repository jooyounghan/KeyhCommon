#pragma once
#include "CommonCore.h"
#include "StringView.h"

namespace keyh
{
	struct JsonUtil
	{
		static constexpr const char kNull[] = "null";
		static constexpr const char kTrue[] = "true";
		static constexpr const char kFalse[] = "false";

		static constexpr size_t kNullLength = sizeof(kNull) - 1;
		static constexpr size_t kTrueLength = sizeof(kTrue) - 1;
		static constexpr size_t kFalseLength = sizeof(kFalse) - 1;

		enum class TapeType : uint8
		{
			ObjectStart = '{',
			ObjectEnd = '}',
			ArrayStart = '[',
			ArrayEnd = ']',
			String = '"',
			Integer = 'i',
			Float = 'f',
			Boolean = 'b',

		};

		class TapeElement
		{
		private:
			uint64 _value;

		public:
			void setElement(TapeType type, uint64 payload);
			void setStringElement(uint64 offset, uint32 length);

		public:
			inline TapeType getType() const { return static_cast<TapeType>(_value >> 56); }
			inline uint64 getPayload() const { return _value & 0x00FFFFFFFFFFFFFFULL; }
			inline uint64 getStringOffset() const { return _value & 0xFFFFFFFF /* lower 32 bits */; }
			inline uint32 getStringLength() const { return static_cast<uint32>((_value >> 32) & 0xFFFFFF /* next 24 bits */); }

		public:
			void setPayload(uint64 payload) { _value = (_value & 0xFF00000000000000ULL) | (payload & 0x00FFFFFFFFFFFFFFULL); }

		public:
			float		parseAsFloat() const;
			int			parseAsInt() const;
			StringViewA	parseAsStringView(const char* jsonString) const;
		};
	};
}