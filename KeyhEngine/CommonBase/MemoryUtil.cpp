#include "CommonBasePch.h"
#include "MemoryUtil.h"

namespace keyh
{
	size_t MemoryUtil::fastBitCeil(size_t value)
	{
		uint32 x = static_cast<uint32>(value + 1);
		unsigned long leadingZeros;

		if (_BitScanReverse(&leadingZeros, x))
		{
			return static_cast<size_t>(1) << (leadingZeros + 1);
		}
		return 1;
	}
}
