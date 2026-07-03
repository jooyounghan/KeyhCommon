#pragma once
#include "JsonUtil.h"
#include "JsonElement.h"
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
		File							_jsonFile;
		Vector<JsonUtil::TapeElement>	_tapeElements;
		bool							_isValid = false;

	private:
		JsonContext						_context;

	public:
		inline bool						isValid() const { return _isValid; }
		JsonObject						getRootObject() const;
	};
}
