#pragma once
#include "CommonCore.h"
#include "Vector.h"
#include "File.h"

namespace keyh
{
	class JsonDocument
	{
	public:
		JsonDocument() = delete;
		explicit JsonDocument(const char* jsonPath)
		{
			_jsonFile.load(jsonPath);
			buildFromJsonString(_jsonFile.getStringBuffer(), _jsonFile.getFileSize());
		}

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
		};

	private:
		File				_jsonFile;
		Vector<TapeElement> _tapeElements;

	private:
		void buildFromJsonString(const char* jsonString, size_t size);
	};
}

