#pragma once
#include "JsonUtil.h"
#include "Span.h"

namespace keyh
{
	template<typename Derived>
	class JsonElement
	{
	public:
		JsonElement(Vector<JsonUtil::TapeElement>& tapeElements, const char* jsonString, size_t index)
			: _tapeElementsView(tapeElements), _jsonString(jsonString), _index(index)
		{
		}

	private:
		Span<JsonUtil::TapeElement> _tapeElementsView;
		const char* _jsonString;
		const size_t _index;
	};

	class JsonObject
	{
	public:
		JsonObject(Vector<JsonUtil::TapeElement>& tapeElements, const char* jsonString, size_t index)
			: _tapeElementsView(tapeElements), _jsonString(jsonString), _index(index)
		{
		
		}

	private:
		Span<JsonUtil::TapeElement> _tapeElementsView;
		const char* _jsonString;
		const size_t _index;

	public:
		JsonObject getNextObject() const
		{
			uint64_t payload = _tapeElementsView[_index].getPayload();

			return JsonObject(_tapeElementsView, _jsonString, _index);
		}

	};
}