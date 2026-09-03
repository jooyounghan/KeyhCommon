#include "MaterialSystemPch.h"
#include "MaterialParameterInfo.h"
#include "MaterialParameterHolder.h"

namespace keyh
{
	KEYH_REFLECT_DEFINE_BODY(MaterialBitFlagParameterInfo)
	KEYH_REFLECT_DEFINE_BODY(MaterialParameterInfo)

	template<typename BitFlag>
	static Ptr<IMaterialParameterHolder> createBitFlagHolder(const Vector<MaterialBitFlagParameterInfo>& bitFlagInfos)
	{
		Ptr<IMaterialParameterHolder> parameterHolder = makePtr<BitFlag>();
		IMaterialParameterHolder_BitFlag* bitFlagHolder = static_cast<IMaterialParameterHolder_BitFlag*>(parameterHolder.get());
		for (const MaterialBitFlagParameterInfo& bitFlagInfo : bitFlagInfos)
		{
			bitFlagHolder->setValue(bitFlagInfo._bitIndex, bitFlagInfo._defaultValue);
		}
		return parameterHolder;
	}

	void MaterialParameterInfo::initializeParameterHolder()
	{
		StringViewA defaultValueView(_defaultValue.c_str(), _defaultValue.size());

		switch (_parameterType)
		{
			case MaterialParameterType::BitFlag8:
			{
				_parameterHolder = createBitFlagHolder<MaterialParameterHolder_BitFlag8>(_bitFlagInfos);
				break;
			}
			case MaterialParameterType::BitFlag16:
			{
				_parameterHolder = createBitFlagHolder<MaterialParameterHolder_BitFlag16>(_bitFlagInfos);
				break;
			}
			case MaterialParameterType::BitFlag32:
			{
				_parameterHolder = createBitFlagHolder<MaterialParameterHolder_BitFlag32>(_bitFlagInfos);
				break;
			}
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