namespace keyh
{
	// Default isEqual: delegates to operator==.
	// Works for all arithmetic types, StaticStringA, and FlyweightStringA.
	// Specialize ReflectPropertyPolicy<T>::isEqual for custom comparison semantics.
	template<typename T>
	bool ReflectPropertyPolicy<T>::isEqual(const T& a, const T& b)
	{
		return a == b;
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToJson(IBuffer* buffer, const T& value)
	{
		static_assert(sizeof(T) == 0,
			"ReflectPropertyPolicy<T>::serializeToJson is not implemented for this type. "
			"Specialize ReflectPropertyPolicy<T> to add JSON serialization support.");
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::deserializeFromJson(const JsonElement& json, T& value)
	{
		static_assert(sizeof(T) == 0,
			"ReflectPropertyPolicy<T>::deserializeFromJson is not implemented for this type. "
			"Specialize ReflectPropertyPolicy<T> to add JSON deserialization support.");
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::serializeToBinary(IBuffer* buffer, const T& value)
	{
		static_assert(sizeof(T) == 0,
			"ReflectPropertyPolicy<T>::serializeToBinary is not implemented for this type. "
			"Specialize ReflectPropertyPolicy<T> to add binary serialization support.");
	}

	template<typename T>
	void ReflectPropertyPolicy<T>::deserializeFromBinary(const void* data, size_t size, T& value)
	{
		static_assert(sizeof(T) == 0,
			"ReflectPropertyPolicy<T>::deserializeFromBinary is not implemented for this type. "
			"Specialize ReflectPropertyPolicy<T> to add binary deserialization support.");
	}

// Declares explicit specializations for a built-in type.
// The definitions live in ReflectPropertyPolicy.cpp.
#define DECLARE_REFLECT_PROPERTY_POLICY(Type)																		\
	template<> void ReflectPropertyPolicy<Type>::serializeToJson(IBuffer* buffer, const Type& value);				\
	template<> void ReflectPropertyPolicy<Type>::deserializeFromJson(const JsonElement& json, Type& value);			\
	template<> void ReflectPropertyPolicy<Type>::serializeToBinary(IBuffer* buffer, const Type& value);				\
	template<> void ReflectPropertyPolicy<Type>::deserializeFromBinary(const void* data, size_t size, Type& value);

	DECLARE_REFLECT_PROPERTY_POLICY(int8)
	DECLARE_REFLECT_PROPERTY_POLICY(int16)
	DECLARE_REFLECT_PROPERTY_POLICY(int32)
	DECLARE_REFLECT_PROPERTY_POLICY(int64)
	DECLARE_REFLECT_PROPERTY_POLICY(uint8)
	DECLARE_REFLECT_PROPERTY_POLICY(uint16)
	DECLARE_REFLECT_PROPERTY_POLICY(uint32)
	DECLARE_REFLECT_PROPERTY_POLICY(uint64)
	DECLARE_REFLECT_PROPERTY_POLICY(float)
	DECLARE_REFLECT_PROPERTY_POLICY(double)
	DECLARE_REFLECT_PROPERTY_POLICY(bool)
	DECLARE_REFLECT_PROPERTY_POLICY(StaticStringA)
	DECLARE_REFLECT_PROPERTY_POLICY(FlyweightStringA)

#undef DECLARE_REFLECT_PROPERTY_POLICY
}
