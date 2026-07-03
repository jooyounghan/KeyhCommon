#pragma once
#include "JsonUtil.h"
#include "Span.h"

namespace keyh
{
	struct JsonContext
	{
		Span<JsonUtil::TapeElement> _tapeElementsView;
		const char*					_jsonString;
	};

	class JsonElement
	{
	public:
		JsonElement() : _context(nullptr), _index(kInvalidSizeT) {}
		JsonElement(const JsonContext* context, size_t index);

	protected:
		const JsonContext*	_context;
		size_t				_index;

	public:
		inline bool							isValid() const { return _context != nullptr; }
		inline const JsonContext*			getContext(size_t index, const size_t* endIndex = nullptr) const { return index >= (endIndex ? *endIndex : _context->_tapeElementsView.size()) ? nullptr : _context; }
		inline const JsonUtil::TapeElement&	getTapeElement() const { return _context->_tapeElementsView[_index]; }
	};

#define JSON_ELEMENT_ACCESSOR	\
using JsonElement::_context;	\
using JsonElement::_index

	class JsonValue : public JsonElement
	{
		friend class JsonKey;
		friend class JsonArray;
		friend class JsonObject;
		JSON_ELEMENT_ACCESSOR;

	public:
		JsonValue(const JsonContext* context, size_t index);

	private:
		size_t getValueEndIndex() const;

	public:
		JsonUtil::TapeType getValueType() const;

	public:
		StringViewA getStringValue() const;
		int			getIntValue() const;
		float		getFloatValue() const;
		bool		getBoolValue() const;
		JsonArray	getArrayValue() const;
		JsonObject	getObjectValue() const;
	};

	class JsonKey : public JsonElement
	{
		JSON_ELEMENT_ACCESSOR;

	public:
		JsonKey(const JsonContext* context, size_t index);

	public:
		StringViewA getKeyName() const;
		JsonValue getValue() const;
	};

	class JsonArray : public JsonElement
	{
		JSON_ELEMENT_ACCESSOR;

	public:
		JsonArray(const JsonContext* context, size_t index);

	public:
		JsonValue getFirstValue() const;
		JsonValue getNextValue(const JsonValue& currentValue) const;
	};

	class JsonObject : public JsonElement
	{
		JSON_ELEMENT_ACCESSOR;

	public:
		JsonObject() : JsonElement(nullptr, kInvalidSizeT) {}
		JsonObject(const JsonContext* context, size_t index);

	public:
		JsonObject	getNextObject() const;
		JsonKey		getFirstKey() const;
		JsonKey		getNextKey(const JsonKey& currentKey) const;
	};
}