#include "CommonBasePch.h"
#include "IBuffer.h"
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

	constexpr StaticArray<char, StrUtil::kDigitMapCount> StrUtil::makeDigitMap()
	{
		StaticArray<char, StrUtil::kDigitMapCount> digitMap;
		for (size_t idx = 0; idx < StrUtil::kDigitMapCount; ++idx)
		{
			digitMap[2 * idx + 0] = '0' + static_cast<char>(idx / 10);
			digitMap[2 * idx + 1] = '0' + static_cast<char>(idx % 10);
		}
		return digitMap;
	}

	constexpr StaticArray<uint64, StrUtil::kPowerOf10Count> StrUtil::makePowerOf10Map()
	{
		StaticArray<uint64, StrUtil::kPowerOf10Count> power10Map;
		power10Map[0] = 1;
		for (uint32 idx = 1; idx < StrUtil::kPowerOf10Count; ++idx)
		{
			power10Map[idx] = power10Map[idx - 1] * 10ull;
		}
		return power10Map;

	}

	void StrUtil::digitToStr(bool isNegative, uint64 value, IBufferBase* buffer)
	{
		constexpr StaticArray<char, kDigitMapCount> digitMap = makeDigitMap();
		constexpr StaticArray<uint64, kPowerOf10Count> power10Map = makePowerOf10Map();
	}

	void StrUtil::intToStr(bool isNegative, uint64 value, IBufferBase* buffer)
	{
		//char temp[32];
		//int len = snprintf(temp, sizeof(temp), "%lld", value);
		//buffer->write(temp, len);	
	}

	void StrUtil::floatToStr(float value, IBufferBase* buffer, int precision)
	{
		//char format[8];
		//snprintf(format, sizeof(format), "%%.%df", precision);
		//char temp[64];
		//int len = snprintf(temp, sizeof(temp), format, value);
		//buffer->write(temp, len);
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