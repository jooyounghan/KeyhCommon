#pragma once
#include "JsonUtil.h"
#include "Vector.h"
#include "File.h"

namespace keyh
{
	class JsonDocument
	{
	public:
		JsonDocument() = default;
		explicit JsonDocument(const char* jsonPath) { loadFromFile(jsonPath); }

	public:
		bool loadFromFile(const char* jsonPath);
		bool buildFromJsonString(const char* jsonString, size_t size);

	private:
		File				_jsonFile;
		Vector<JsonUtil::TapeElement> _tapeElements;
		bool				_isValid = false;

	public:
		inline bool isValid() const { return _isValid; }
		inline size_t getTapeElementCount() const { return _tapeElements.size(); }
		inline const Vector<JsonUtil::TapeElement>& getTapeElements() const { return _tapeElements; }
		inline const char* getJsonStringBuffer() const { return _jsonFile.getStringBuffer(); }
	};
}
