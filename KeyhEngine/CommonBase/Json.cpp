#include "CommonBasePch.h"
#include "Json.h"
#include "StrUtil.h"
#include "Stack.h"
#include "StaticString.h"

namespace keyh
{
    void JsonDocument::TapeElement::setElement(TapeType type, uint64 payload)
    {
        uint64 typeVal = static_cast<uint64>(type);
        _value = (typeVal << 56) | (payload & 0x00FFFFFFFFFFFFFFULL);
    }
    
    void JsonDocument::TapeElement::setStringElement(uint64 offset, uint32 length)
    {
        uint64 typeVal = static_cast<uint64>(TapeType::String);
        uint64 lengthVal = static_cast<uint64>(length);
        _value = (typeVal << 56) | ((lengthVal & 0xFFFFFF) << 32) | (offset & 0xFFFFFFFF);
    }

	static void handleError(Vector<JsonDocument::TapeElement>& tapeElements, bool condition, const char* errorMessage)
	{
		if (!condition)
		{
			KEYH_ASSERT_DEV(false, errorMessage);
			tapeElements.clear();
		}
	}

	static void handleTapeOpen(Vector<JsonDocument::TapeElement>& tapeElements, Stack<size_t>& indexStack, JsonDocument::TapeType type)
	{
		indexStack.push(tapeElements.size());
		tapeElements.emplace_back().setElement(type, 0);
	}

	static bool handleTapeClose(Vector<JsonDocument::TapeElement>& tapeElements, Stack<size_t>& indexStack, JsonDocument::TapeType type)
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

	static bool handleStringElement(Vector<JsonDocument::TapeElement>& tapeElements, const char* jsonString, const char* quoteStart, const char* quoteEnd)
	{
		if (!quoteStart || !quoteEnd || quoteEnd <= quoteStart)
		{
			return false;
		}

		tapeElements.emplace_back().setStringElement(quoteStart - jsonString + 1, static_cast<uint32_t>(quoteEnd - quoteStart - 1));
		return true;
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
				handleTapeOpen(_tapeElements, objectIndexStack, TapeType::ObjectStart);
				break;
			}
			case '}':
			{
				if (!handleTapeClose(_tapeElements, objectIndexStack, TapeType::ObjectEnd))
				{
					return handleError(_tapeElements, false, "Mismatched closing brace '}' in JSON string");
				}
				break;
			}
			case '[':
			{
				handleTapeOpen(_tapeElements, arrayIndexStack, TapeType::ArrayStart);
				break;
			}
			case ']':
			{
				if (!handleTapeClose(_tapeElements, arrayIndexStack, TapeType::ArrayEnd))
				{
					return handleError(_tapeElements, false, "Mismatched closing bracket ']' in JSON string");
				}
				break;
			}
			case '"':
			{
				const char* quoteStart = ptr;
				const char* quoteEnd = StrUtil::findNext(ptr + 1, end, '"');

				if (!handleStringElement(_tapeElements, jsonString, quoteStart, quoteEnd))
				{
					return handleError(_tapeElements, false, "Invalid string in JSON string");
				}

				ptr = quoteEnd;
				break;
			}
			default:
			{
				if (StrUtil::isDigit(c) || c == '-' || c == '+')
				{
					const char* current = ptr + 1;
					bool hasDecimalPoint = false;
					while (current < end)
					{
						if (StrUtil::isDigit(*current) || *current == '-' || *current == '+')
						{
							++current;
						}
						else if (*current == '.')
						{
							++current;
							hasDecimalPoint = true;
						}
						else
						{
							break;
						}
					}

					StaticStringA numberStr(ptr, current - ptr);
					if (hasDecimalPoint)
					{
						float numberValue = StrUtil::strToFloat(numberStr.c_str());
						uint32 bitPattern = 0;
						memcpy(&bitPattern, &numberValue, sizeof(float));
						_tapeElements.emplace_back().setElement(TapeType::Float, static_cast<uint64>(bitPattern));
					}
					else
					{
						int numberValue = StrUtil::strToInt(numberStr.c_str());
						_tapeElements.emplace_back().setElement(TapeType::Integer, numberValue);
					}
					ptr = current - 1; // Move ptr to the end of the number
				}
				else if (StrUtil::strcmp(ptr, "true") == 0)
				{
					_tapeElements.emplace_back().setElement(TapeType::Boolean, 1);
					ptr += 3; // Move ptr to the end of "true"

				}
				else if (StrUtil::strcmp(ptr, "false") == 0)
				{
					_tapeElements.emplace_back().setElement(TapeType::Boolean, 0);
					ptr += 4; // Move ptr to the end of "false"
				}
				else
				{
					handleError(_tapeElements, false, "Invalid character in JSON string");
				}
				break;
			}
			}
			ptr = StrUtil::skipWhiteSpace(ptr + 1, end);
		}
    }
}