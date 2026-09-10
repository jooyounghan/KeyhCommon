#include "MaterialSystemPch.h"
#include "MaterialParameterView.h"

namespace keyh
{
#define CREATE_UPDATE_DESC(FunctionName, ParameterType, DataType) \
	MaterialParameterUpdateDesc MaterialParameterUpdateDesc::FunctionName(const FlyweightStringA& parameterName, const DataType* data) \
	{ \
		MaterialParameterUpdateDesc desc; \
		desc._parameterName = parameterName; \
		desc._parameterType = MaterialParameterType::ParameterType; \
		desc._data = data; \
		return desc; \
	}

	CREATE_UPDATE_DESC(createIntUpdateDesc, Int, int)
	CREATE_UPDATE_DESC(createInt2UpdateDesc, Int2, int2)
	CREATE_UPDATE_DESC(createFloatUpdateDesc, Float, float)
	CREATE_UPDATE_DESC(createFloat2UpdateDesc, Float2, float2)
	CREATE_UPDATE_DESC(createFloat3UpdateDesc, Float3, float3)
	CREATE_UPDATE_DESC(createFloat4UpdateDesc, Float4, float4)
}
