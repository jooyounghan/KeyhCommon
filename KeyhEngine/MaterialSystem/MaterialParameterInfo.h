#pragma once
#include "ReflectSerializer.h"

namespace keyh
{
	KEYH_REFLECT_ENUM
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
#include "MaterialParameterInfo.reflect_generated.inl"
