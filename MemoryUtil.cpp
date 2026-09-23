#include "CommonBasePch.h"
#include "MemoryUtil.h"

namespace keyh
{
	constexpr size_t kBitsPerByte = 8;

	void* MemoryUtil::alignedAlloc(size_t size, size_t alignment)
	{
#if defined(_MSC_VER)
		return _aligned_malloc(size, alignment);
#else
		void* ptr = nullptr;
		if (posix_memalign(&ptr, alignment, size) != 0)
		{
			return nullptr;
		}
		return ptr;
#endif
	}

	void MemoryUtil::alignedFree(void* ptr)
	{
#if defined(_MSC_VER)
		_aligned_free(ptr);
#else
		free(ptr);
#endif
	}

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
