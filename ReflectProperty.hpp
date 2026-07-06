#include "ReflectProperty.h"
namespace keyh
{
	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::serializeToBuffer(IBuffer* buffer)
	{
		ReflectSerializer::serializeToBuffer<ValueType>((this->*_refGetter)(), buffer);
	}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::deserializeFromJson(const JsonElement & jsonElement)
	{}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::serializeToBinary(IBuffer * buffer)
	{}

	template<typename ObjectType, typename ValueType>
	void ReflectProperty<ObjectType, ValueType>::deserializeFromBinary(const void* data, size_t size)
	{}


}