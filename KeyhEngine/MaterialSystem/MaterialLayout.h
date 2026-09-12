#pragma once
#include "MaterialParameterView.h"

namespace keyh
{
	using MaterialBitFlagViews = HashMap<FlyweightStringA, uint32>;
	using MaterialParameterViews = HashMap<FlyweightStringA, MaterialParameterView>;

	class REFLECTIVE(MaterialLayout)
	{
		KEYH_REFLECT_DECLARE_BODY(MaterialLayout)

#ifdef KEYH_DEV
		friend class MaterialDefinition;
#endif

	private:
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialBitFlagViews")
		MaterialBitFlagViews _materialBitFlagViews;

		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialParameterViews")
		MaterialParameterViews _materialParameterViews;
	
		KEYH_REFLECT_PROPERTY(PropertyName = "MaterialMemorySize")
		DynamicBuffer<byte> _gpuMaterialMemoryBlock;

	public:
		void updateMaterialBitFlag(const FlyweightStringA & bitFlagName, bool value);
		void updateMaterialParameter(const MaterialParameterUpdateDesc& updateDesc);
	};
}

#include "MaterialLayout.reflect_generated.inl"
