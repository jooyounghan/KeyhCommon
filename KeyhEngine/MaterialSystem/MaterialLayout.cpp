#include "MaterialSystemPch.h"
#include "MaterialLayout.h"

namespace keyh
{
	void MaterialLayout::updateMaterialBitFlag(const FlyweightStringA& bitFlagName, bool value)
	{
		MaterialBitFlagViews::FindResult findResult = _materialBitFlagViews.find(bitFlagName);
		if (findResult.isFound() == false)
		{
			KEYH_ASSERT_DEV(false, "Bit flag not found");
			return;
		}

		const uint32 bitOffset = *findResult.value();

		const uint32 byteIndex = bitOffset / 8;
		const byte bitMask = static_cast<byte>(1 << (bitOffset % 8));

		if (byteIndex >= _gpuMaterialMemoryBlock.size())
		{
			KEYH_ASSERT_DEV(false, "Bit offset out of range");
			return;
		}

		byte* gpuBuffer = _gpuMaterialMemoryBlock.getBuffer();
		if (value)
			gpuBuffer[byteIndex] |= bitMask;
		else
			gpuBuffer[byteIndex] &= ~bitMask;
	}

	void MaterialLayout::updateMaterialParameter(const MaterialParameterUpdateDesc& updateDesc)
	{
		MaterialParameterViews::FindResult findResult = _materialParameterViews.find(updateDesc.getParameterName());
		if (findResult.isFound() == false)
		{
			KEYH_ASSERT_DEV(false, "Bit flag not found");
			return;
		}

		const MaterialParameterView& materialParameterView = *findResult.value();
		MaterialParameterType materialParameterType = materialParameterView.getParameterType();
		if (materialParameterType != updateDesc.getParameterType())
		{
			KEYH_ASSERT_DEV(false, "Material Parameter Type Dismatch");
			return;
		}

		byte* gpuBuffer = _gpuMaterialMemoryBlock.getBuffer();

		const uint32 offset = materialParameterView.getOffset();
		memcpy(&gpuBuffer[offset], updateDesc.getData(), getParamTypeGpuMemorySize(materialParameterType));
	}

}
