#pragma once
#include "CommonCore.h"
#include "Vector.h"

namespace keyh
{
	class JsonDocument
	{
		enum class TapeType : uint8
		{
			ObjectStart = '{',
			ObjectEnd = '}',
			ArrayStart = '[',
			ArrayEnd = ']',
			String = '"',
			Number = '0',
		};

		class TapeElement
		{
		private:
			uint64 _value;

		public:
            void setElement(TapeType type, uint64_t payload);
            void setStringElement(uint64_t offset, uint32_t length);

		public:
            inline TapeType getType() const { return static_cast<TapeType>(_value >> 56); }
            inline uint64_t getPayload() const { return _value & 0x00FFFFFFFFFFFFFFULL; }
            inline uint64_t getStringOffset() const { return _value & 0xFFFFFFFF; }
            inline uint32_t getStringLength() const { return static_cast<uint32_t>((_value >> 32) & 0xFFFFFF); }
		};

	private:
		Vector<TapeElement> _tapeElements;

	public:
		void buildFromJsonString(const char* jsonString);
	};
}

