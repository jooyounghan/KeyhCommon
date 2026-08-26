#pragma once
#include "ReflectSerializer.h"

namespace keyh
{
	enum class EMaterialParamType
	{
		Int,
		Int2,
		Float,
		Float2,
		Float3,
		Float4,
		Matrix4x4,
		TextureReferencePath,
	};

	KEYH_REFLECT_ENUM_BEGIN(EMaterialParamType)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Int)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Int2)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Float)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Float2)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Float3)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Float4)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, Matrix4x4)
		KEYH_REFLECT_ENUM_VALUE(EMaterialParamType, TextureReferencePath)
		KEYH_REFLECT_ENUM_END(EMaterialParamType)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Int)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Int2)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Float)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Float2)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Float3)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Float4)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, Matrix4x4)
		KEYH_REFLECT_ENUM_VALUE_FROM_STRING(EMaterialParamType, TextureReferencePath)
	KEYH_REFLECT_ENUM_END_FROM_STRING()

	class REFLECTIVE(MaterialParameterInfo)
	{
		KEYH_REFLECT_BODY(MaterialParameterInfo)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "ParameterName")
		FlyweightStringA _parameterNames;

		KEYH_REFLECT_PROPERTY(PropertyName = "ParameterType")
		EMaterialParamType _parameterType = EMaterialParamType::Int;

	};


}
