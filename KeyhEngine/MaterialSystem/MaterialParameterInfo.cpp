#include "MaterialSystemPch.h"
#include "MaterialParameterInfo.h"
#include "MaterialParameterHolder.h"

namespace keyh
{
	void MaterialParameterInfo::initializeParameterHolder()
	{
		StringViewA defaultValueView(_defaultValue.c_str(), _defaultValue.size());

		switch (_parameterType)
		{
			case MaterialParameterType::Int:
			{
				_parameterHolder = makePtr<MaterialParameterHolder<int>>(StrUtil::strToInt<int>(_defaultValue.c_str()));
				break;
			}
			case MaterialParameterType::Int2:
			{
				//SplitterStringA splitter(defaultValueView, ",");
				//_parameterHolder = makePtr<MaterialParameterHolder<int2>>();
				break;
			}
			case MaterialParameterType::Float:
			{
				//_parameterHolder = makePtr<MaterialParameterHolder<float>>();
				break;
			}
			case MaterialParameterType::Float2:
			{
				//_parameterHolder = makePtr<MaterialParameterHolder<float2>>();
				break;
			}
			case MaterialParameterType::Float3:
			{
				//_parameterHolder = makePtr<MaterialParameterHolder<float3>>();
				break;
			}
			case MaterialParameterType::Float4:
			{
				//_parameterHolder = makePtr<MaterialParameterHolder<float4>>();
				break;
			}
			case MaterialParameterType::Transform:
			{
				KEYH_ASSERT_DEV(false, "Not Implemented");
				break;
			}
			case MaterialParameterType::ReferencedPath_Texture:
			{
				//_parameterHolder = makePtr<MaterialParamaterHolderRefencedPath_Texture>();
				break;
			}
			default:
			{
				KEYH_ASSERT_DEV(false, "Unknown Material Parameter Type");
				break;
			}
		}
	}
}