#include "ReflectSerializer.h"
namespace keyh
{
	template<typename T>
	void ReflectSerializer::serializeToBuffer(const T& object, IBuffer* buffer)
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(T);
	}

	template<typename T>
	void ReflectSerializer::deserializeFromBuffer(const void* buffer, T& object)
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(T);
	}

#pragma region serializeToBuffer
#define DECLARE_SERIALIZE_TO_BUFFER(Type) \
	template <> \
	void ReflectSerializer::serializeToBuffer<Type>(const Type& object, IBuffer* buffer)

	DECLARE_SERIALIZE_TO_BUFFER(int8);
	DECLARE_SERIALIZE_TO_BUFFER(int16);
	DECLARE_SERIALIZE_TO_BUFFER(int32);
	DECLARE_SERIALIZE_TO_BUFFER(int64);
	DECLARE_SERIALIZE_TO_BUFFER(uint8);
	DECLARE_SERIALIZE_TO_BUFFER(uint16);
	DECLARE_SERIALIZE_TO_BUFFER(uint32);
	DECLARE_SERIALIZE_TO_BUFFER(uint64);

	DECLARE_SERIALIZE_TO_BUFFER(float);
	DECLARE_SERIALIZE_TO_BUFFER(double);

	DECLARE_SERIALIZE_TO_BUFFER(bool);
	DECLARE_SERIALIZE_TO_BUFFER(StaticStringA);
	DECLARE_SERIALIZE_TO_BUFFER(FlyweightStringA);

#undef DECLARE_SERIALIZE_TO_BUFFER
#pragma endregion

#pragma region deserializeFromBuffer
#define DECLARE_DESERIALIZE_TO_BUFFER(Type) \
	template <> \
	void ReflectSerializer::deserializeFromBuffer(const void* buffer, Type& object)

	DECLARE_DESERIALIZE_TO_BUFFER(int8);
	DECLARE_DESERIALIZE_TO_BUFFER(int16);
	DECLARE_DESERIALIZE_TO_BUFFER(int32);
	DECLARE_DESERIALIZE_TO_BUFFER(int64);
	DECLARE_DESERIALIZE_TO_BUFFER(uint8);
	DECLARE_DESERIALIZE_TO_BUFFER(uint16);
	DECLARE_DESERIALIZE_TO_BUFFER(uint32);
	DECLARE_DESERIALIZE_TO_BUFFER(uint64);

	DECLARE_DESERIALIZE_TO_BUFFER(float);
	DECLARE_DESERIALIZE_TO_BUFFER(double);

	DECLARE_DESERIALIZE_TO_BUFFER(bool);
	DECLARE_DESERIALIZE_TO_BUFFER(StaticStringA);
	DECLARE_DESERIALIZE_TO_BUFFER(FlyweightStringA);

#undef DECLARE_SERIALIZE_TO_BUFFER
#pragma endregion
}