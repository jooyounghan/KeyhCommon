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

		static constexpr size_t kIndexMask = 0xFFFFFFFF;				// lower 32 bits

		static constexpr size_t kStringOffsetMask = 0xFFFFFFFF;			// lower 32 bits
		static constexpr size_t kStringLengthMask = 0xFFFFFF;			// next 24 bits
		static constexpr size_t kStringLengthShift = 32;				// shift for string length

		static constexpr uint64 kPayloadMask = 0x00FFFFFFFFFFFFFFULL;	// lower 56 bits
		static constexpr uint64 kTypeMask = 0xFF00000000000000ULL;		// upper 8 bits
		static constexpr size_t kTypeShift = 56;						// shift for type (upper 8 bits)

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
			void setIndexElement(TapeType type, size_t index);
			void setStringElement(size_t offset, size_t length);

		public:
			inline TapeType getType() const { return static_cast<TapeType>(_value >> kTypeShift); }

			inline size_t getPayloadAsIndex() const { return static_cast<size_t>(_value & kIndexMask); }
			inline size_t getPayloadAsStringOffset() const { return static_cast<size_t>(_value & kStringOffsetMask); }
			inline size_t getPayloadAsStringLength() const { return static_cast<size_t>((_value >> kStringLengthShift) & kStringLengthMask); }

		public:
			uint64 getPayload() const { return _value & kPayloadMask; }
			void setPayload(uint64 payload) { _value = (_value & kTypeMask) | (payload & kPayloadMask); }

		public:
			template<typename T>
			T parse() const;

			template<>
			inline float parse<float>() const { return parseImpl<float>(); }

			template<>
			inline int parse<int>() const { return parseImpl<int>(); }

			template<>
			inline bool parse<bool>() const { return parseImpl<bool>(); }

		public:
			StringViewA	parse(const char* jsonString) const;

		private:
			template<typename T>
			T parseImpl() const;
		};
	};
}