namespace keyh
{
#pragma region Base
	template<typename T>
	bool ReflectPropertyPolicy<T>::isEqual(const T& a, const T& b)
	{
		return ReflectSerializer<T>::isEqual(a, b);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToJson(IBuffer* buffer, const T& value)
	{
		return ReflectSerializer<T>::serializeToJson(buffer, value);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::deserializeFromJson(const JsonValue& json, T& value)
	{
		return ReflectSerializer<T>::deserializeFromJson(json, value);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToBinary(IBuffer* buffer, const T& value)
	{
		return ReflectSerializer<T>::serializeToBinary(buffer, value);
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::deserializeFromBinary(const void* data, size_t size, T& value)
	{
		return ReflectSerializer<T>::deserializeFromBinary(data, size, value);
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
	void ReflectPropertyPolicy<Vector<ElementType>>::serializeToJson(IBuffer* buffer, const Vector<ElementType>& value)
	{}
	
	template<typename ElementType>
	void ReflectPropertyPolicy<Vector<ElementType>>::deserializeFromJson(const JsonValue& json, Vector<ElementType>&value)
	{}
	
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
		return false;
	}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::serializeToJson(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value)
	{}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::deserializeFromJson(const JsonValue& json, HashMap<KeyType, ValueType, Hasher>& value)
	{}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::serializeToBinary(IBuffer* buffer, const HashMap<KeyType, ValueType, Hasher>& value)
	{}
	
	template<typename KeyType, typename ValueType, typename Hasher>
	void ReflectPropertyPolicy<HashMap<KeyType, ValueType, Hasher>>::deserializeFromBinary(const void* data, size_t size, HashMap<KeyType, ValueType, Hasher>& value)
	{}
#pragma endregion
}
