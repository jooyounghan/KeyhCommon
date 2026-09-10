#pragma once
#include "MaterialParameterView.h"

namespace keyh
{
	class REFLECTIVE(MaterialLayout)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialLayout)

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialBitFlagViews")
		Vector<MaterialParameterBitFlagView> _materialBitFlagViews;

		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialParameterViews")
		Vector<MaterialParameterView> _materialParameterViews;
	
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialMemorySize")
		DynamicBuffer<byte> _gpuMaterialMemoryBlock;

	public:
		void updateMaterialBitFlag(const FlyweightStringA & bitFlagName, bool value);
		void updateMaterialParameter(const MaterialParameterUpdateDesc& updateDesc);
	};
}

