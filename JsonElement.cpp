#include "CommonBasePch.h"
#include "JsonElement.h"

namespace keyh
{
	using TapeType = JsonUtil::TapeType;
	using TapeElement = JsonUtil::TapeElement;

#pragma region JsonElement
	JsonElement::JsonElement(const JsonContext* context, size_t index)
		: _context(context), _index(index)
	{}
#pragma endregion

#pragma region JsonValue
	JsonValue::JsonValue(const JsonContext* context, size_t index)
		: JsonElement(context, index)
	{}

	size_t JsonValue::getValueEndIndex() const
	{
		size_t endIndex = kInvalidSizeT;
		const TapeElement& element = getTapeElement();
		TapeType type = element.getType();
		switch (type)
		{
		case TapeType::ObjectStart:
		case TapeType::ArrayStart:
			endIndex = element.getPayloadAsIndex();
			break;
		case TapeType::String:
		case TapeType::Integer:
		case TapeType::Float:
		case TapeType::Boolean:
			endIndex = _index;
			break;
		default:
			break;
		}

		return endIndex;
	}

	TapeType JsonValue::getValueType() const
	{
		const TapeElement& element = getTapeElement();
		return element.getType();
	}

	StringViewA JsonValue::getStringValue() const
	{
		const TapeElement& element = getTapeElement();
		return element.parse(_context->_jsonString);
	}

	int JsonValue::getIntValue() const
	{
		const TapeElement& element = getTapeElement();
		return element.parse<int>();
	}

	float JsonValue::getFloatValue() const
	{
		const TapeElement& element = getTapeElement();
		return element.parse<float>();
	}

	bool JsonValue::getBoolValue() const
	{
		const TapeElement& element = getTapeElement();
		return element.parse<bool>();
	}

	JsonArray JsonValue::getArrayValue() const
	{
		return JsonArray(getContext(_index), _index);
	}

	JsonObject JsonValue::getObjectValue() const
	{
		if (isValid() == false)
			return JsonObject(nullptr, 0);

		return JsonObject(getContext(_index), _index);
	}
#pragma endregion

#pragma region JsonKey
	JsonKey::JsonKey(const JsonContext* context, size_t index)
		: JsonElement(context, index)
	{}

	JsonValue JsonKey::getValue() const
	{
		if (isValid() == false)
			return JsonValue(nullptr, 0);

		return JsonValue(getContext(_index + 1), _index + 1);
	}

	StringViewA JsonKey::getKeyName() const
	{
		return getTapeElement().parse(_context->_jsonString);
	}

	size_t JsonKey::getValueEndIndex() const
	{
		const size_t valueIndex = _index + 1;
		const TapeElement& valueElement = _context->_tapeElementsView[valueIndex];
		const TapeType valueType = valueElement.getType();
		if (valueType == TapeType::ObjectStart || valueType == TapeType::ArrayStart)
			return valueElement.getPayloadAsIndex();
		else
			return valueIndex;
	}
#pragma endregion

	JsonArray::JsonArray(const JsonContext* context, size_t index)
		: JsonElement(context, index)
	{}

	JsonValue JsonArray::getFirstValue() const
	{
		if (isValid() == false)
			return JsonValue(nullptr, 0);

		const TapeElement& element = getTapeElement();
		size_t arrayEndIndex = element.getPayloadAsIndex();
		return JsonValue(getContext(_index + 1, &arrayEndIndex), _index + 1);
	}

	JsonValue JsonArray::getNextValue(const JsonValue& currentValue) const
	{
		if (currentValue.isValid() == false)
			return JsonValue(nullptr, 0);

		const TapeElement& element = getTapeElement();
		size_t arrayEndIndex = element.getPayloadAsIndex();
		size_t endIndex = currentValue.getValueEndIndex();
		return JsonValue(getContext(endIndex + 1, &arrayEndIndex), endIndex + 1);
	}


	JsonObject::JsonObject(const JsonContext* context, size_t index)
		: JsonElement(context, index)
	{

	}

	JsonObject JsonObject::getNextObject() const
	{
		if (isValid() == false)
			return JsonObject(nullptr, 0);

		const TapeElement& element = getTapeElement();
		size_t index = element.getPayloadAsIndex();
		return JsonObject(getContext(index + 1), index + 1);
	}

	JsonKey JsonObject::getFirstKey() const
	{
		if (isValid() == false)
			return JsonKey(nullptr, 0);

		const TapeElement& element = getTapeElement();
		size_t objectEndIndex = element.getPayloadAsIndex();
		return JsonKey(getContext(_index + 1, &objectEndIndex), _index + 1);
	}

	JsonKey JsonObject::getNextKey(const JsonKey& currentKey) const
	{
		if (currentKey.isValid() == false)
			return JsonKey(nullptr, 0);

		const TapeElement& element = getTapeElement();
		size_t objectEndIndex = element.getPayloadAsIndex();
		size_t endIndex = currentKey.getValueEndIndex();
		return JsonKey(getContext(endIndex + 1, &objectEndIndex), endIndex + 1);
	}

}
