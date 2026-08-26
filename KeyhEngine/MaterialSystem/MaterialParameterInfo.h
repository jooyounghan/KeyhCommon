#pragma once

namespace keyh
{
	class REFLECTIVE(MaterialParameterInfo)
	{
		KEYH_REFLECT_BODY(MaterialParameterInfo)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "ParameterName")
		FlyweightStringA _parameterNames;

	};


}
#include "MaterialParameterInfo.reflect_generated.inl"
