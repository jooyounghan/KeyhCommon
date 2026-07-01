#include "CommonBasePch.h"
#include "Json.h"
#include "StrUtil.h"
#include "Stack.h"

namespace keyh
{
	static void handleError(Vector<JsonUtil::TapeElement>& tapeElements, bool condition, const char* errorMessage)
	{
		if (!condition)
		{
			KEYH_ASSERT_DEV(false, errorMessage);
			tapeElements.clear();
		}
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
		if (StrUtil::strcmp(ptr, JsonUtil::kTrue) == 0)
		{
			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Boolean, 1);
			ptr += JsonUtil::kTrueLength;
			return true;
		}
		else if (StrUtil::strcmp(ptr, JsonUtil::kFalse) == 0)
		{
			tapeElements.emplace_back().setElement(JsonUtil::TapeType::Boolean, 0);
			ptr += JsonUtil::kFalseLength;
			return true;
		}
		return false;
	}

    void JsonDocument::buildFromJsonString(const char* jsonString, size_t size)
    {
		const char* const end = jsonString + size;
		const char* ptr = StrUtil::skipWhiteSpace(jsonString, end);

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
				if (startWithDigitOrSign && handleNumberElement(_tapeElements, ptr, end) == false)
				{
					return handleError(_tapeElements, false, "Invalid number in JSON string");
				}
				else if (handleBooleanElement(_tapeElements, ptr, end) == false)
				{
					return handleError(_tapeElements, false, "Invalid Type in JSON string");
				}
				break;
			}
			}
			ptr = StrUtil::skipWhiteSpace(ptr + 1, end);
		}
    }
}