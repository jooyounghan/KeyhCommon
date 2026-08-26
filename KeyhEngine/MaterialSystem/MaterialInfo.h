#pragma once

namespace keyh
{
	class MaterialParameterInfo;

	class REFLECTIVE(MaterialInfo)
	{
		KEYH_REFLECT_BODY(MaterialInfo)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialName")
		FlyweightStringA _materialName;

		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialParameterInfos")
		OwnerVector<MaterialParameterInfo> _materialParameterInfos;
	};
}
#include "MaterialInfo.reflect_generated.inl"
