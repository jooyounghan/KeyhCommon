#include "CommonBasePch.h"
#include "StrUtil.h"
#include "SimdUtil.h"

namespace keyh
{
	int StrUtil::strToInt(const char* str, int radix, char** endPtr)
	{
		return ::strtoll(str, endPtr, radix);
	}
	float StrUtil::strToFloat(const char* str, char** endPtr)
	{
		return ::strtof(str, endPtr);
	}

	const char* StrUtil::findNext(const char* start, const char* end, const char target)
	{
		const char* pos = start;

#if defined(SIMD_ENABLED)
		SimdUtil::SimdVec targetVec = SimdUtil::set8(target);
		while (end - pos >= SimdUtil::kSimdTargetByte)
		{
			SimdUtil::SimdVec chunk = SimdUtil::load(pos);
			SimdUtil::SimdMask mask = SimdUtil::cmpEpi8(chunk, targetVec);
			if (mask != 0)
			{
				unsigned int index = SimdUtil::bitScanForward(mask);
				return pos + index;
			}
			pos += SimdUtil::kSimdTargetByte;
		}
#endif

		while (pos < end && *pos != target) ++pos;
		return pos;
	}

	const char* StrUtil::skipWhiteSpace(const char* start, const char* end)
	{
		const char* pos = start;

#if defined(SIMD_ENABLED)
		const SimdUtil::SimdVec spaceVec = SimdUtil::set8(' ');
		const SimdUtil::SimdVec tabVec = SimdUtil::set8('\t');
		const SimdUtil::SimdVec newlineVec = SimdUtil::set8('\n');
		const SimdUtil::SimdVec carriageReturnVec = SimdUtil::set8('\r');

		while (end - pos >= SimdUtil::kSimdTargetByte)
		{
			SimdUtil::SimdVec chunk = SimdUtil::load(pos);

			SimdUtil::SimdMask mask = SimdUtil::cmpEpi8(chunk, spaceVec)
				| SimdUtil::cmpEpi8(chunk, tabVec)
				| SimdUtil::cmpEpi8(chunk, newlineVec)
				| SimdUtil::cmpEpi8(chunk, carriageReturnVec);

			if (mask == SIMD_ALL_ONES_MASK)
			{
				pos += SimdUtil::kSimdTargetByte;
				continue;
			}
			break;
		}

#endif
		while (pos < end && isWhitespace(*pos)) ++pos;
		return pos;
	}
}