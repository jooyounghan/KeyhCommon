#pragma once
#include "CommonCore.h"
#include "Vector.h"
#include "StringView.h"
#include "DynamicBuffer.h"
namespace keyh
{
	using TapeElement = uint64;

	class JsonDocument
	{
	private:
		Vector<TapeElement> _tapeElements;
		DynamicBuffer<char> _jsonString;

	public:
		void buildFromJsonString(const char* jsonString);
	};
}

