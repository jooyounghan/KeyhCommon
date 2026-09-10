#include "MaterialSystemPch.h"
#include "MaterialLayout.h"

namespace keyh
{
	void MaterialLayout::updateMaterialBitFlag(const FlyweightStringA& bitFlagName, bool value)
	{
		for (const MaterialParameterBitFlagView& bitFlagView : _materialBitFlagViews)
		{
			if (bitFlagName != bitFlagView.getBitFlagName())
				continue;

			const uint32 bitOffset = bitFlagView.getBitOffset();
			const uint32 byteIndex = bitOffset / 8;
			const byte bitMask = static_cast<byte>(1 << (bitOffset % 8));

			if (byteIndex >= _gpuMaterialMemoryBlock.size())
			{
				KEYH_ASSERT_DEV(false, "Bit offset out of range");
				return;
			}

			if (value)
				_gpuMaterialMemoryBlock[byteIndex] |= bitMask;
			else
				_gpuMaterialMemoryBlock[byteIndex] &= ~bitMask;

			return;
		}
	}

	void MaterialLayout::updateMaterialParameter(const MaterialParameterUpdateDesc& updateDesc)
	{
	
	}

}
