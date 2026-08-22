namespace keyh
{
#pragma region Base
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
	void ReflectPropertyPolicy<T>::deserializeFromBinary(const void* data, size_t size, T& value)
	{
		return ReflectPropertySerializer<T>::deserializeFromBinary(data, size, value);
	}
#pragma endregion

#pragma region Vector Policy
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
	{}
	template<typename ElementType>
	
	void ReflectPropertyPolicy<Vector<ElementType>>::deserializeFromBinary(const void* data, size_t size, Vector<ElementType>&value)
	{}
#pragma endregion

#pragma region HashMap Policy
	template<typename KeyType, typename ValueType, typename Hasher>
	bool ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::isEqual(const HashMap<KeyType, ValueType, Hasher>& a, const HashMap<KeyType, ValueType, Hasher>& b)
	{
		if (a.size() != b.size())
			return false;

		for (const auto& bucket : a)
		{
			auto findResult = b.find(bucket.key());
			if (!findResult.isFound())
				return false;

			if (!ReflectPropertyPolicy<ValueType>::isEqual(bucket.value(), *findResult.value()))
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
		for (const auto& bucket : value)
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
			buffer->writeBytes("k", 1);
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
			buffer->writeBytes("v", 1);
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
				if (keyName == "k")
				{
					ReflectPropertyPolicy<KeyType>::deserializeFromJson(jsonKey.getValue(), k);
					hasKey = true;
				}
				else if (keyName == "v")
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
	{}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::deserializeFromBinary(const void* data, size_t size, HashMap<KeyType, ValueType, Hasher>& value)
	{}
#pragma endregion
}
