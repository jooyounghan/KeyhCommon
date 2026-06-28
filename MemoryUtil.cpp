#include "CommonBasePch.h"
#include "MemoryUtil.h"

namespace keyh
{
	constexpr size_t kBitsPerByte = 8;

	size_t MemoryUtil::fastBitCeil(size_t value)
	{
		if (value <= 1)
		{
			return 1;
		}

		--value;
		for (size_t shift = 1; shift < sizeof(size_t) * kBitsPerByte; shift <<= 1)
		{
			value |= value >> shift;
		}
		return value + 1;
	}
}
