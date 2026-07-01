#pragma once
#include "JsonUtil.h"
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

	private:
		File				_jsonFile;
		Vector<JsonUtil::TapeElement> _tapeElements;

	private:
		void buildFromJsonString(const char* jsonString, size_t size);
	};
}

