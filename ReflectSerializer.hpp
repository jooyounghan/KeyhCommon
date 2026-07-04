#define DECLARE_SERIALIZE_TO_JSON(Type) \
	template <> \
	void ReflectSerializer::serializeToJson<Type>(const Type& object)

namespace keyh
{
	template<typename T>
	void ReflectSerializer::serializeToJson(const T& object)
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(T, "The type is not supported for serialization. Please implement the serializeToJson method for this type.");
	}

	DECLARE_SERIALIZE_TO_JSON(int8);
	DECLARE_SERIALIZE_TO_JSON(int16);
	DECLARE_SERIALIZE_TO_JSON(int32);
	DECLARE_SERIALIZE_TO_JSON(int64);
	DECLARE_SERIALIZE_TO_JSON(uint8);
	DECLARE_SERIALIZE_TO_JSON(uint16);
	DECLARE_SERIALIZE_TO_JSON(uint32);
	DECLARE_SERIALIZE_TO_JSON(uint64);
	DECLARE_SERIALIZE_TO_JSON(float);
	DECLARE_SERIALIZE_TO_JSON(double);
	DECLARE_SERIALIZE_TO_JSON(bool);
	DECLARE_SERIALIZE_TO_JSON(StaticStringA);
	DECLARE_SERIALIZE_TO_JSON(FlyweightStringA);
}