#include "ReflectPropertyPolicy.h"
namespace keyh
{
#pragma region Base
	template<typename T>
	bool ReflectPropertyPolicy<T>::isDefault(const T& value)
	{
		return isEqual(T{}, value);
	}

	template<typename T>
	bool ReflectPropertyPolicy<T>::isEqual(const T& a, const T& b)
	{
		return ReflectPropertySerializer<T>::isEqual(a, b);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToJson(IBuffer* buffer, const T& value, size_t depth, bool pretty)
	{
		return ReflectPropertySerializer<T>::serializeToJson(buffer, value, depth, pretty);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::deserializeFromJson(const JsonValue& json, T& value)
	{
		return ReflectPropertySerializer<T>::deserializeFromJson(json, value);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToBinary(IBuffer* buffer, const T& value)
	{
		return ReflectPropertySerializer<T>::serializeToBinary(buffer, value);
	}

	template<typename T>
	size_t ReflectPropertyPolicy<T>::deserializeFromBinary(const void* data, size_t size, T& value)
	{
		return ReflectPropertySerializer<T>::deserializeFromBinary(data, size, value);
	}
#pragma endregion

#pragma region Ptr Policy
	template<typename ElementType>
	bool ReflectPropertyPolicy<Ptr<ElementType>>::isDefault(const Ptr<ElementType>& value)
	{
		return value.get() == nullptr;
	}

	template<typename ElementType>
	bool ReflectPropertyPolicy<Ptr<ElementType>>::isEqual(const Ptr<ElementType>& a, const Ptr<ElementType>& b)
	{
		if (a.get() == nullptr || b.get() == nullptr)
			return a.get() == b.get();
		return ReflectPropertyPolicy<ElementType>::isEqual(*a.get(), *b.get());
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<Ptr<ElementType>>::serializeToJson(IBuffer* buffer, const Ptr<ElementType>& value, size_t depth, bool pretty)
	{
		const ElementType* element = value.get();
		if (element == nullptr)
		{
			buffer->writeBytes("\"null\"", 6);
			return;
		}
		if constexpr (IsReflectObject_v<ElementType>)
		{
			const StringViewA typeName = ReflectTypeRegistry<ElementType>::findName(*element);
			if (!typeName.empty())
			{
				buffer->writeBytes("{\"$type\":\"", 10);
				buffer->writeBytes(typeName.c_str(), typeName.size());
				buffer->writeBytes("\",\"$value\":", 11);
				ReflectPropertyPolicy<ElementType>::serializeToJson(buffer, *element, depth + 1, pretty);
				buffer->writeBytes("}", 1);
				return;
			}
		}
		ReflectPropertyPolicy<ElementType>::serializeToJson(buffer, *element, depth, pretty);
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<Ptr<ElementType>>::deserializeFromJson(const JsonValue& json, Ptr<ElementType>& value)
	{
		if (!json.isValid())
			return;
		if (json.getValueType() == JsonUtil::TapeType::String && json.getStringValue() == "null")
		{
			value = nullptr;
			return;
		}
		Ptr<ElementType> result;
		JsonValue payload = json;
		if constexpr (IsReflectObject_v<ElementType>)
		{
			if (json.getValueType() != JsonUtil::TapeType::ObjectStart)
				return;
			const JsonObject object = json.getObjectValue();
			StringViewA typeName;
			bool hasType = false;
			bool hasValue = false;
			size_t keyCount = 0;
			for (JsonKey key = object.getFirstKey(); key.isValid(); key = object.getNextKey(key))
			{
				++keyCount;
				if (key.getKeyName() == "$type")
				{
					if (hasType || key.getValue().getValueType() != JsonUtil::TapeType::String)
						return;
					hasType = true;
					typeName = key.getValue().getStringValue();
				}
				else if (key.getKeyName() == "$value")
				{
					if (hasValue || key.getValue().getValueType() != JsonUtil::TapeType::ObjectStart)
						return;
					hasValue = true;
					payload = key.getValue();
				}
			}
			if ((hasType || hasValue) && (!hasType || !hasValue || keyCount != 2))
				return;
			result = hasType ? ReflectTypeRegistry<ElementType>::create(typeName)
				: ReflectTypeRegistry<ElementType>::createLegacy();
		}
		else
		{
			result = makePtr<ElementType>();
		}
		if (result.get() == nullptr)
			return;
		ReflectPropertyPolicy<ElementType>::deserializeFromJson(payload, *result.get());
		value = keyh::move(result);
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<Ptr<ElementType>>::serializeToBinary(IBuffer* buffer, const Ptr<ElementType>& value)
	{
		const ElementType* element = value.get();
		if constexpr (IsReflectObject_v<ElementType>)
		{
			if (element != nullptr)
			{
				const StringViewA typeName = ReflectTypeRegistry<ElementType>::findName(*element);
				if (!typeName.empty())
				{
					const uint8 typed = 2;
					const uint64 nameSize = typeName.size();
					buffer->writeBytes(&typed, sizeof(typed));
					buffer->writeBytes(&nameSize, sizeof(nameSize));
					buffer->writeBytes(typeName.c_str(), typeName.size());
					ReflectPropertyPolicy<ElementType>::serializeToBinary(buffer, *element);
					return;
				}
			}
		}
		const uint8 present = element != nullptr ? 1 : 0;
		buffer->writeBytes(&present, sizeof(present));
		if (element != nullptr)
			ReflectPropertyPolicy<ElementType>::serializeToBinary(buffer, *element);
	}

	template<typename ElementType>
	size_t ReflectPropertyPolicy<Ptr<ElementType>>::deserializeFromBinary(const void* data, size_t size, Ptr<ElementType>& value)
	{
		if (data == nullptr || size == 0)
			return kInvalidSizeT;
		const uint8 present = *static_cast<const uint8*>(data);
		if (present > 2)
			return kInvalidSizeT;
		if (present == 0)
		{
			value = nullptr;
			return sizeof(present);
		}
		size_t offset = sizeof(present);
		Ptr<ElementType> result;
		if constexpr (IsReflectObject_v<ElementType>)
		{
			if (present == 2)
			{
				uint64 nameSize = 0;
				if (size - offset < sizeof(nameSize))
					return kInvalidSizeT;
				memcpy(&nameSize, static_cast<const byte*>(data) + offset, sizeof(nameSize));
				offset += sizeof(nameSize);
				if (nameSize == 0 || nameSize > size - offset)
					return kInvalidSizeT;
				const StringViewA typeName(static_cast<const char*>(data) + offset, static_cast<size_t>(nameSize));
				result = ReflectTypeRegistry<ElementType>::create(typeName);
				offset += static_cast<size_t>(nameSize);
			}
			else
				result = ReflectTypeRegistry<ElementType>::createLegacy();
		}
		else
		{
			if (present != 1)
				return kInvalidSizeT;
			result = makePtr<ElementType>();
		}
		if (result.get() == nullptr)
			return kInvalidSizeT;
		const size_t consumed = ReflectPropertyPolicy<ElementType>::deserializeFromBinary(
			static_cast<const byte*>(data) + offset, size - offset, *result.get());
		if (consumed == kInvalidSizeT || consumed > size - offset)
			return kInvalidSizeT;
		value = keyh::move(result);
		return offset + consumed;
	}
#pragma endregion

#pragma region Vector Policy
	template<typename ElementType>
	bool ReflectPropertyPolicy<Vector<ElementType>>::isDefault(const Vector<ElementType>& value)
	{
		return value.empty();
	}

	template<typename ElementType>
	bool ReflectPropertyPolicy<Vector<ElementType>>::isEqual(const Vector<ElementType>& a, const Vector<ElementType>& b)
	{
		if (a.size() != b.size())
			return false;

		for (size_t i = 0; i < a.size(); ++i)
		{
			if (ReflectPropertyPolicy<ElementType>::isEqual(a[i], b[i]) == false)
				return false;
		}
		return true;
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<Vector<ElementType>>::serializeToJson(IBuffer* buffer, const Vector<ElementType>& value, size_t depth, bool pretty)
	{
		buffer->writeBytes(&ReflectionUtil::kArrayBegin, 1);
		if (pretty && value.empty() == false)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
		}
		for (size_t i = 0; i < value.size(); ++i)
		{
			if (i > 0)
			{
				buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
				if (pretty)
				{
					const char newline = '\n';
					buffer->writeBytes(&newline, 1);
				}
			}

			if (pretty)
			{
				for (size_t indent = 0; indent < depth + 1; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}

			ReflectPropertyPolicy<ElementType>::serializeToJson(buffer, value[i], depth + 1, pretty);
		}
		if (pretty && value.empty() == false)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
			for (size_t indent = 0; indent < depth; ++indent)
			{
				buffer->writeBytes("  ", 2);
			}
		}
		buffer->writeBytes(&ReflectionUtil::kArrayEnd, 1);
	}
	
	template<typename ElementType>
	void ReflectPropertyPolicy<Vector<ElementType>>::deserializeFromJson(const JsonValue& json, Vector<ElementType>&value)
	{
		JsonArray jsonArray = json.getArrayValue();
		for (JsonValue jsonValue = jsonArray.getFirstValue(); jsonValue.isValid(); jsonValue = jsonArray.getNextValue(jsonValue))
		{
			ElementType& element = value.push_back(ElementType());
			ReflectPropertyPolicy<ElementType>::deserializeFromJson(jsonValue, element);
		}
	}
	
	template<typename ElementType>
	void ReflectPropertyPolicy<Vector<ElementType>>::serializeToBinary(IBuffer * buffer, const Vector<ElementType>&value)
	{
		const uint64 count = value.size();
		buffer->writeBytes(&count, sizeof(count));
		for (const ElementType& element : value)
		{
			ReflectPropertyPolicy<ElementType>::serializeToBinary(buffer, element);
		}
	}
	template<typename ElementType>
	
	size_t ReflectPropertyPolicy<Vector<ElementType>>::deserializeFromBinary(const void* data, size_t size, Vector<ElementType>&value)
	{
		uint64 count = 0;
		if (data == nullptr || size < sizeof(count))
			return kInvalidSizeT;
		memcpy(&count, data, sizeof(count));
		size_t offset = sizeof(count);
		Vector<ElementType> result;
		for (uint64 i = 0; i < count; ++i)
		{
			ElementType& element = result.push_back(ElementType{});
			const size_t consumed = ReflectPropertyPolicy<ElementType>::deserializeFromBinary(static_cast<const byte*>(data) + offset, size - offset, element);
			if (consumed == kInvalidSizeT || consumed > size - offset)
				return kInvalidSizeT;
			offset += consumed;
		}
		value = keyh::move(result);
		return offset;
	}
#pragma endregion

#pragma region OwnerVector Policy
	template<typename ElementType>
	bool ReflectPropertyPolicy<OwnerVector<ElementType>>::isDefault(const OwnerVector<ElementType>& value)
	{
		return value.size() == 0;
	}

	template<typename ElementType>
	bool ReflectPropertyPolicy<OwnerVector<ElementType>>::isEqual(const OwnerVector<ElementType>& a, const OwnerVector<ElementType>& b)
	{
		if (a.size() != b.size())
			return false;

		for (size_t i = 0; i < a.size(); ++i)
		{
			const ElementType* left = a[i];
			const ElementType* right = b[i];
			if (left == nullptr || right == nullptr)
			{
				if (left != right)
					return false;
				continue;
			}

			if (!ReflectPropertyPolicy<ElementType>::isEqual(*left, *right))
				return false;
		}
		return true;
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<OwnerVector<ElementType>>::serializeToJson(IBuffer* buffer, const OwnerVector<ElementType>& value, size_t depth, bool pretty)
	{
		buffer->writeBytes(&ReflectionUtil::kArrayBegin, 1);
		if (pretty && value.size() > 0)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
		}

		for (size_t i = 0; i < value.size(); ++i)
		{
			if (i > 0)
			{
				buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
				if (pretty)
				{
					const char newline = '\n';
					buffer->writeBytes(&newline, 1);
				}
			}

			if (pretty)
			{
				for (size_t indent = 0; indent < depth + 1; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}

			const ElementType* element = value[i];
			if (element == nullptr)
			{
				buffer->writeBytes(&ReflectionUtil::kQuote, 1);
				buffer->writeBytes("null", 4);
				buffer->writeBytes(&ReflectionUtil::kQuote, 1);
				continue;
			}
			if constexpr (IsReflectObject_v<ElementType>)
			{
				const StringViewA typeName = ReflectTypeRegistry<ElementType>::findName(*element);
				if (!typeName.empty())
				{
					buffer->writeBytes("{\"$type\":\"", 10);
					buffer->writeBytes(typeName.c_str(), typeName.size());
					buffer->writeBytes("\",\"$value\":", 11);
					ReflectPropertyPolicy<ElementType>::serializeToJson(buffer, *element, depth + 1, pretty);
					buffer->writeBytes("}", 1);
					continue;
				}
			}
			ReflectPropertyPolicy<ElementType>::serializeToJson(buffer, *element, depth + 1, pretty);
		}

		if (pretty && value.size() > 0)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
			for (size_t indent = 0; indent < depth; ++indent)
			{
				buffer->writeBytes("  ", 2);
			}
		}
		buffer->writeBytes(&ReflectionUtil::kArrayEnd, 1);
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<OwnerVector<ElementType>>::deserializeFromJson(const JsonValue& json, OwnerVector<ElementType>& value)
	{
		const StringViewA rawJson = json.getRawJsonValue();
		KEYH_ASSERT_ARGS(json.isValid() && json.getValueType() == JsonUtil::TapeType::ArrayStart,
			"OwnerVector JSON value must be an array (type %u, value '%.*s').",
			static_cast<unsigned int>(json.isValid() ? json.getValueType() : JsonUtil::TapeType::ObjectEnd),
			static_cast<int>(rawJson.length()), rawJson.data() != nullptr ? rawJson.data() : "");
		if (!json.isValid() || json.getValueType() != JsonUtil::TapeType::ArrayStart)
			return;
		OwnerVector<ElementType> result;
		JsonArray jsonArray = json.getArrayValue();
		for (JsonValue jsonValue = jsonArray.getFirstValue(); jsonValue.isValid(); jsonValue = jsonArray.getNextValue(jsonValue))
		{
			if (jsonValue.getValueType() == JsonUtil::TapeType::String && jsonValue.getStringValue() == "null")
			{
				result.push_back(Ptr<ElementType>(nullptr));
				continue;
			}

			if constexpr (IsReflectObject_v<ElementType>)
			{
				if (jsonValue.getValueType() != JsonUtil::TapeType::ObjectStart)
					return;
				const JsonObject object = jsonValue.getObjectValue();
				JsonValue payload = jsonValue;
				StringViewA typeName;
				bool hasType = false;
				bool hasValue = false;
				size_t keyCount = 0;
				for (JsonKey key = object.getFirstKey(); key.isValid(); key = object.getNextKey(key))
				{
					++keyCount;
					if (key.getKeyName() == "$type")
					{
						if (hasType || key.getValue().getValueType() != JsonUtil::TapeType::String)
							return;
						hasType = true;
						typeName = key.getValue().getStringValue();
					}
					else if (key.getKeyName() == "$value")
					{
						if (hasValue || key.getValue().getValueType() != JsonUtil::TapeType::ObjectStart)
							return;
						hasValue = true;
						payload = key.getValue();
					}
				}
				if ((hasType || hasValue) && (!hasType || !hasValue || keyCount != 2))
					return;
				Ptr<ElementType> owned = hasType ? ReflectTypeRegistry<ElementType>::create(typeName)
					: ReflectTypeRegistry<ElementType>::createLegacy();
				ElementType* element = owned.get();
				if (element == nullptr)
					return;
				result.push_back(keyh::move(owned));
				ReflectPropertyPolicy<ElementType>::deserializeFromJson(payload, *element);
			}
			else
			{
				ElementType* element = result.template emplace_back<ElementType>();
				ReflectPropertyPolicy<ElementType>::deserializeFromJson(jsonValue, *element);
			}
		}
		value = keyh::move(result);
	}

	template<typename ElementType>
	void ReflectPropertyPolicy<OwnerVector<ElementType>>::serializeToBinary(IBuffer* buffer, const OwnerVector<ElementType>& value)
	{
		const uint64 count = value.size();
		buffer->writeBytes(&count, sizeof(count));
		for (const ElementType* element : value)
		{
			if constexpr (IsReflectObject_v<ElementType>)
			{
				if (element != nullptr)
				{
					const StringViewA typeName = ReflectTypeRegistry<ElementType>::findName(*element);
					if (!typeName.empty())
					{
						const uint8 typed = 2;
						const uint64 nameSize = typeName.size();
						buffer->writeBytes(&typed, sizeof(typed));
						buffer->writeBytes(&nameSize, sizeof(nameSize));
						buffer->writeBytes(typeName.c_str(), typeName.size());
						ReflectPropertyPolicy<ElementType>::serializeToBinary(buffer, *element);
						continue;
					}
				}
			}
			const uint8 present = element != nullptr ? 1 : 0;
			buffer->writeBytes(&present, sizeof(present));
			if (element != nullptr)
			{
				ReflectPropertyPolicy<ElementType>::serializeToBinary(buffer, *element);
			}
		}
	}

	template<typename ElementType>
	size_t ReflectPropertyPolicy<OwnerVector<ElementType>>::deserializeFromBinary(const void* data, size_t size, OwnerVector<ElementType>& value)
	{
		uint64 count = 0;
		if (data == nullptr || size < sizeof(count))
			return kInvalidSizeT;
		memcpy(&count, data, sizeof(count));
		size_t offset = sizeof(count);
		OwnerVector<ElementType> result;
		for (uint64 i = 0; i < count; ++i)
		{
			if (offset >= size)
				return kInvalidSizeT;
			const uint8 present = static_cast<const byte*>(data)[offset++];
			if (present > 2)
				return kInvalidSizeT;
			if (present == 0)
			{
				result.push_back(Ptr<ElementType>(nullptr));
				continue;
			}
			ElementType* element = nullptr;
			if constexpr (IsReflectObject_v<ElementType>)
			{
				Ptr<ElementType> owned;
				if (present == 2)
				{
					uint64 nameSize = 0;
					if (size - offset < sizeof(nameSize))
						return kInvalidSizeT;
					memcpy(&nameSize, static_cast<const byte*>(data) + offset, sizeof(nameSize));
					offset += sizeof(nameSize);
					if (nameSize == 0 || nameSize > size - offset)
						return kInvalidSizeT;
					const StringViewA typeName(static_cast<const char*>(data) + offset, static_cast<size_t>(nameSize));
					owned = ReflectTypeRegistry<ElementType>::create(typeName);
					offset += static_cast<size_t>(nameSize);
				}
				else
					owned = ReflectTypeRegistry<ElementType>::createLegacy();
				element = owned.get();
				if (element == nullptr)
					return kInvalidSizeT;
				result.push_back(keyh::move(owned));
			}
			else
			{
				if (present != 1)
					return kInvalidSizeT;
				element = result.template emplace_back<ElementType>();
			}
			const size_t consumed = ReflectPropertyPolicy<ElementType>::deserializeFromBinary(static_cast<const byte*>(data) + offset, size - offset, *element);
			if (consumed == kInvalidSizeT || consumed > size - offset)
				return kInvalidSizeT;
			offset += consumed;
		}
		value = keyh::move(result);
		return offset;
	}
#pragma endregion

#pragma region HashMap Policy
	template<typename KeyType, typename ValueType, typename Hasher>
	bool ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::isDefault(const HashMap<KeyType, ValueType, Hasher>& value)
	{
		return value.size() == 0;
	}

	template<typename KeyType, typename ValueType, typename Hasher>
	bool ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::isEqual(const HashMap<KeyType, ValueType, Hasher>& a, const HashMap<KeyType, ValueType, Hasher>& b)
	{
		if (a.size() != b.size())
			return false;

		for (const typename HashMap<KeyType, ValueType, Hasher>::Bucket& bucket : a)
		{
			typename HashMap<KeyType, ValueType, Hasher>::ConstFindResult findResult = b.find(bucket.key());
			if (findResult.isFound() == false)
				return false;

			if (ReflectPropertyPolicy<ValueType>::isEqual(bucket.value(), *findResult.value()) == false)
				return false;
		}
		return true;
	}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::serializeToJson(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value, size_t depth, bool pretty)
	{
		buffer->writeBytes(&ReflectionUtil::kArrayBegin, 1);
		if (pretty && value.empty() == false)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
		}
		bool isFirst = true;
		for (const typename HashMap<KeyType, ValueType, Hasher>::Bucket& bucket : value)
		{
			if (!isFirst)
			{
				buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
				if (pretty)
				{
					const char newline = '\n';
					buffer->writeBytes(&newline, 1);
				}
			}
			isFirst = false;

			if (pretty)
			{
				for (size_t indent = 0; indent < depth + 1; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}

			buffer->writeBytes(&ReflectionUtil::kObjectBegin, 1);
			if (pretty)
			{
				const char newline = '\n';
				buffer->writeBytes(&newline, 1);
				for (size_t indent = 0; indent < depth + 2; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}

			buffer->writeBytes(&ReflectionUtil::kQuote, 1);
			buffer->writeBytes("Key", 3);
			buffer->writeBytes(&ReflectionUtil::kQuote, 1);
			buffer->writeBytes(&ReflectionUtil::kValueBegin, 1);
			if (pretty)
				buffer->writeBytes(" ", 1);
			ReflectPropertyPolicy<KeyType>::serializeToJson(buffer, bucket.key(), depth + 2, pretty);

			buffer->writeBytes(&ReflectionUtil::kDelimiter, 1);
			if (pretty)
			{
				const char newline = '\n';
				buffer->writeBytes(&newline, 1);
				for (size_t indent = 0; indent < depth + 2; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}

			buffer->writeBytes(&ReflectionUtil::kQuote, 1);
			buffer->writeBytes("Value", 5);
			buffer->writeBytes(&ReflectionUtil::kQuote, 1);
			buffer->writeBytes(&ReflectionUtil::kValueBegin, 1);
			if (pretty)
				buffer->writeBytes(" ", 1);
			ReflectPropertyPolicy<ValueType>::serializeToJson(buffer, bucket.value(), depth + 2, pretty);

			if (pretty)
			{
				const char newline = '\n';
				buffer->writeBytes(&newline, 1);
				for (size_t indent = 0; indent < depth + 1; ++indent)
				{
					buffer->writeBytes("  ", 2);
				}
			}
			buffer->writeBytes(&ReflectionUtil::kObjectEnd, 1);
		}
		if (pretty && value.empty() == false)
		{
			const char newline = '\n';
			buffer->writeBytes(&newline, 1);
			for (size_t indent = 0; indent < depth; ++indent)
			{
				buffer->writeBytes("  ", 2);
			}
		}
		buffer->writeBytes(&ReflectionUtil::kArrayEnd, 1);
	}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::deserializeFromJson(const JsonValue& json, HashMap<KeyType, ValueType, Hasher>& value)
	{
		value.clear();
		JsonArray jsonArray = json.getArrayValue();
		for (JsonValue jsonEntry = jsonArray.getFirstValue(); jsonEntry.isValid(); jsonEntry = jsonArray.getNextValue(jsonEntry))
		{
			JsonObject entryObj = jsonEntry.getObjectValue();
			if (!entryObj.isValid())
				continue;

			KeyType k;
			ValueType v;
			bool hasKey = false;
			bool hasValue = false;
			for (JsonKey jsonKey = entryObj.getFirstKey(); jsonKey.isValid(); jsonKey = entryObj.getNextKey(jsonKey))
			{
				StringViewA keyName = jsonKey.getKeyName();
				if (keyName == "Key")
				{
					ReflectPropertyPolicy<KeyType>::deserializeFromJson(jsonKey.getValue(), k);
					hasKey = true;
				}
				else if (keyName == "Value")
				{
					ReflectPropertyPolicy<ValueType>::deserializeFromJson(jsonKey.getValue(), v);
					hasValue = true;
				}
			}
			if (hasKey && hasValue)
				value.insert(keyh::move(k), keyh::move(v));
		}
	}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::serializeToBinary(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value)
	{
		const uint64 count = value.size();
		buffer->writeBytes(&count, sizeof(count));
		for (const typename HashMap<KeyType, ValueType, Hasher>::Bucket& bucket : value)
		{
			ReflectPropertyPolicy<KeyType>::serializeToBinary(buffer, bucket.key());
			ReflectPropertyPolicy<ValueType>::serializeToBinary(buffer, bucket.value());
		}
	}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	size_t ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::deserializeFromBinary(const void* data, size_t size, HashMap<KeyType, ValueType, Hasher>& value)
	{
		uint64 count = 0;
		if (data == nullptr || size < sizeof(count))
			return kInvalidSizeT;
		memcpy(&count, data, sizeof(count));
		size_t offset = sizeof(count);
		HashMap<KeyType, ValueType, Hasher> result;
		for (uint64 i = 0; i < count; ++i)
		{
			KeyType key{};
			ValueType element{};
			const size_t keyConsumed = ReflectPropertyPolicy<KeyType>::deserializeFromBinary(static_cast<const byte*>(data) + offset, size - offset, key);
			if (keyConsumed == kInvalidSizeT || keyConsumed > size - offset)
				return kInvalidSizeT;
			offset += keyConsumed;
			const size_t valueConsumed = ReflectPropertyPolicy<ValueType>::deserializeFromBinary(static_cast<const byte*>(data) + offset, size - offset, element);
			if (valueConsumed == kInvalidSizeT || valueConsumed > size - offset)
				return kInvalidSizeT;
			offset += valueConsumed;
			if (result.find(key).isFound())
				return kInvalidSizeT;
			result.insert(keyh::move(key), keyh::move(element));
		}
		value = keyh::move(result);
		return offset;
	}
#pragma endregion
}
