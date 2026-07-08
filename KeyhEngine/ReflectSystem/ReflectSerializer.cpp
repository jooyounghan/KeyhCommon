#include "ReflectSystemPch.h"
#include "ReflectSerializer.h"
#include "IBuffer.h"

namespace keyh
{
#pragma region serializeToBuffer
#define DEFINE_SERIALIZE_TO_BUFFER_INT(Type)	\
	template<> void ReflectSerializer::serializeToBuffer<Type>(const Type& property, IBuffer* buffer) {	\
		bool isNegative = property >= 0.f;	\
		StrUtil::intToStr(isNegative, static_cast<uint64>(property), buffer);	\
	}

#define DEFINE_SERIALIZE_TO_BUFFER_FLOAT(Type)	\
	template<> void ReflectSerializer::serializeToBuffer<Type>(const Type& property, IBuffer* buffer) {	\
		StrUtil::floatToStr(static_cast<double>(property), buffer);	\
	}

	DEFINE_SERIALIZE_TO_BUFFER_INT(int8);
	DEFINE_SERIALIZE_TO_BUFFER_INT(int16);
	DEFINE_SERIALIZE_TO_BUFFER_INT(int32);
	DEFINE_SERIALIZE_TO_BUFFER_INT(int64);
	DEFINE_SERIALIZE_TO_BUFFER_INT(uint8);
	DEFINE_SERIALIZE_TO_BUFFER_INT(uint16);
	DEFINE_SERIALIZE_TO_BUFFER_INT(uint32);
	DEFINE_SERIALIZE_TO_BUFFER_INT(uint64);

	DEFINE_SERIALIZE_TO_BUFFER_FLOAT(float);
	DEFINE_SERIALIZE_TO_BUFFER_FLOAT(double);

#undef DEFINE_SERIALIZE_TO_BUFFER_INT
#undef DEFINE_SERIALIZE_TO_BUFFER_FLOAT

	template<>
	void ReflectSerializer::serializeToBuffer<bool>(const bool& property, IBuffer* buffer)
	{
		constexpr char kTrue[] = "true";
		constexpr char kFalse[] = "false";
		constexpr size_t kTrueSize = sizeof(kTrue);
		constexpr size_t kFalseSize = sizeof(kFalse);
		if (property)
		{
			buffer->writeBytes(kTrue, kTrueSize);
		}
		else
		{
			buffer->writeBytes(kFalse, kFalseSize);
		}
	}

	template<>
	void ReflectSerializer::serializeToBuffer<StaticStringA>(const StaticStringA& property, IBuffer* buffer)
	{
		buffer->writeBytes(property.c_str(), property.size());
	}

	template<>
	void ReflectSerializer::serializeToBuffer<FlyweightStringA>(const FlyweightStringA& property, IBuffer* buffer)
	{
		buffer->writeBytes(property.c_str(), property.size());
	}
#pragma endregion

#pragma region deserializeFromBuffer
#define DEFINE_DESERIALIZE_TO_BUFFER_INT(Type)	\
	template<> void ReflectSerializer::deserializeFromBuffer<Type>(const void* buffer, Type& object) {	\
		object = StrUtil::strToInt<Type>(static_cast<const char*>(buffer));	\
	}

#define DEFINE_DESERIALIZE_TO_BUFFER_FLOAT(Type)	\
	template<> void ReflectSerializer::deserializeFromBuffer<Type>(const void* buffer, Type& object) {	\
		object = StrUtil::strToFloat<Type>(static_cast<const char*>(buffer));	\
	}

	DEFINE_DESERIALIZE_TO_BUFFER_INT(int8);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(int16);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(int32);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(int64);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(uint8);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(uint16);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(uint32);
	DEFINE_DESERIALIZE_TO_BUFFER_INT(uint64);

	DEFINE_DESERIALIZE_TO_BUFFER_FLOAT(float);
	DEFINE_DESERIALIZE_TO_BUFFER_FLOAT(double);

#undef DEFINE_DESERIALIZE_TO_BUFFER_INT
#undef DEFINE_DESERIALIZE_TO_BUFFER_FLOAT

	template<>
	void ReflectSerializer::deserializeFromBuffer<bool>(const void* buffer, bool& object) {
		const char* str = static_cast<const char*>(buffer);
		object = StrUtil::strcmp(str, "true") == 0;
	}

	template<>
	void ReflectSerializer::deserializeFromBuffer<StaticStringA>(const void* buffer, StaticStringA& object) 
	{
		object = StaticStringA(static_cast<const char*>(buffer));
	}

	template<>
	void ReflectSerializer::deserializeFromBuffer<FlyweightStringA>(const void* buffer, FlyweightStringA& object) 
	{
		object = FlyweightStringA(static_cast<const char*>(buffer));
	}

#pragma endregion
}