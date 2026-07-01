#include "CommonBasePch.h"
#include "Json.h"
#include "StrUtil.h"
#include "Stack.h"

namespace keyh
{
	static bool handleError(Vector<JsonUtil::TapeElement>& tapeElements, bool condition, const char* errorMessage)
	{
		if (!condition)
		{
			KEYH_ASSERT_DEV(false, errorMessage);
			tapeElements.clear();
			return false;
		}
		return true;
	}

	static void handleTapeOpen(Vector<JsonUtil::TapeElement>& tapeElements, Stack<size_t>& indexStack, JsonUtil::TapeType type)
	{
		indexStack.push(tapeElements.size());
		tapeElements.emplace_back().setElement(type, 0);
	}

	static bool handleTapeClose(Vector<JsonUtil::TapeElement>& tapeElements, Stack<size_t>& indexStack, JsonUtil::TapeType type)
	{
		if (indexStack.empty())
		{
			return false;
		}

		size_t lastIndex = indexStack.top();
		indexStack.pop();

		tapeElements[lastIndex].setPayload(tapeElements.size());
		tapeElements.emplace_back().setElement(type, lastIndex);

		return true;
	}

	static bool handleStringElement(Vector<JsonUtil::TapeElement>& tapeElements, const char* jsonString, const char* quoteStart, const char* quoteEnd)
	{
		if (!quoteStart || !quoteEnd || quoteEnd <= quoteStart)
		{
			return false;
		}

		tapeElements.emplace_back().setStringElement(quoteStart - jsonString + 1, static_cast<uint32_t>(quoteEnd - quoteStart - 1));
		return true;
	}

	static bool handleNumberElement(Vector<JsonUtil::TapeElement>& tapeElements, const char*& ptr, const char* end)
	{
		const char* current = ptr + 1;
		bool hasDecimalPoint = false;
		while (current < end)
		{
			if (StrUtil::isDigit(*current))
			{
				++current;
			}
			else if (*current == '.')
			{
				if (hasDecimalPoint)
					return false;

				++current;
				hasDecimalPoint = true;
			}
			else
			{
				break;
			}
		}

		if (hasDecimalPoint)
		{
			float numberValue = StrUtil::strToFloat(ptr);
			uint32 bitPattern = 0;
			memcpy(&bitPattern, &numberValue, sizeof(float));
			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Float, static_cast<uint64>(bitPattern));
		}
		else
		{
			int numberValue = StrUtil::strToInt(ptr);
			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Integer, numberValue);
		}
		ptr = current - 1;
		return true;
	}

	static bool handleBooleanElement(Vector<JsonUtil::TapeElement>& tapeElements, const char*& ptr, const char* end)
	{
		const size_t remainingLength = static_cast<size_t>(end - ptr);
		if (remainingLength >= JsonUtil::kTrueLength && memcmp(ptr, JsonUtil::kTrue, JsonUtil::kTrueLength) == 0)
		{
			const char* next = ptr + JsonUtil::kTrueLength;
			if (next != end && !StrUtil::isWhitespace(*next) && *next != ',' && *next != '}' && *next != ']')
			{
				return false;
			}

			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Boolean, 1);
			ptr += JsonUtil::kTrueLength - 1;
			return true;
		}
		else if (remainingLength >= JsonUtil::kFalseLength && memcmp(ptr, JsonUtil::kFalse, JsonUtil::kFalseLength) == 0)
		{
			const char* next = ptr + JsonUtil::kFalseLength;
			if (next != end && !StrUtil::isWhitespace(*next) && *next != ',' && *next != '}' && *next != ']')
			{
				return false;
			}

			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Boolean, 0);
			ptr += JsonUtil::kFalseLength - 1;
			return true;
		}
		return false;
	}

	bool JsonDocument::loadFromFile(const char* jsonPath)
	{
		if (!_jsonFile.load(jsonPath))
		{
			_tapeElements.clear();
			_isValid = false;
			return false;
		}

		return buildFromJsonString(_jsonFile.getStringBuffer(), _jsonFile.getFileSize());
	}

    bool JsonDocument::buildFromJsonString(const char* jsonString, size_t size)
    {
		_tapeElements.clear();
		_isValid = false;

		if (jsonString == nullptr || size == 0)
		{
			return handleError(_tapeElements, false, "JSON string is empty");
		}

		const char* const end = jsonString + size;
		const char* start = jsonString;
		if (size >= 3
			&& static_cast<unsigned char>(start[0]) == 0xEF
			&& static_cast<unsigned char>(start[1]) == 0xBB
			&& static_cast<unsigned char>(start[2]) == 0xBF)
		{
			start += 3;
		}

		const char* ptr = StrUtil::skipWhiteSpace(start, end);

		Stack<size_t> objectIndexStack;
		Stack<size_t> arrayIndexStack;

		while (ptr != end)
		{
			char c = *ptr;
			switch (c)
			{
			case ':':
			case ',':
				// Ignore colons and commas in the JSON string
				break;
			case '{':
			{
				handleTapeOpen(_tapeElements, objectIndexStack, JsonUtil::TapeType::ObjectStart);
				break;
			}
			case '}':
			{
				if (handleTapeClose(_tapeElements, objectIndexStack, JsonUtil::TapeType::ObjectEnd) == false)
				{
					return handleError(_tapeElements, false, "Mismatched closing brace '}' in JSON string");
				}
				break;
			}
			case '[':
			{
				handleTapeOpen(_tapeElements, arrayIndexStack, JsonUtil::TapeType::ArrayStart);
				break;
			}
			case ']':
			{
				if (handleTapeClose(_tapeElements, arrayIndexStack, JsonUtil::TapeType::ArrayEnd) == false)
				{
					return handleError(_tapeElements, false, "Mismatched closing bracket ']' in JSON string");
				}
				break;
			}
			case '"':
			{
				const char* quoteStart = ptr;
				const char* quoteEnd = StrUtil::findNext(ptr + 1, end, '"');

				if (handleStringElement(_tapeElements, jsonString, quoteStart, quoteEnd) == false)
				{
					return handleError(_tapeElements, false, "Invalid string in JSON string");
				}

				ptr = quoteEnd;
				break;
			}
			default:
			{
				const bool startWithDigitOrSign = StrUtil::isDigit(c) || c == '-' || c == '+';
				if (startWithDigitOrSign)
				{
					if (handleNumberElement(_tapeElements, ptr, end) == false)
					{
						return handleError(_tapeElements, false, "Invalid number in JSON string");
					}
				}
				else if (handleBooleanElement(_tapeElements, ptr, end) == false)
				{
					return handleError(_tapeElements, false, "Invalid type in JSON string");
				}
				break;
			}
			}
			ptr = StrUtil::skipWhiteSpace(ptr + 1, end);
		}

		if (!objectIndexStack.empty())
		{
			return handleError(_tapeElements, false, "Object is not closed in JSON string");
		}

		if (!arrayIndexStack.empty())
		{
			return handleError(_tapeElements, false, "Array is not closed in JSON string");
		}

		if (_tapeElements.size() == 0)
		{
			return handleError(_tapeElements, false, "JSON string has no valid tokens");
		}

		_isValid = true;
		return true;
    }
}