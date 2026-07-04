#include "CommonBasePch.h"
#include "IBuffer.h"
#include "StrUtil.h"
#include "MathUtil.h"
#include "SimdUtil.h"
#include "IBuffer.h"

namespace keyh
{
	static constexpr size_t kDigitMapCount = 200;
	static constexpr size_t kDigitMapHalfCount = kDigitMapCount / 2;
	static constexpr size_t kPowerOf10Count = 20;

	constexpr StaticArray<char, kDigitMapCount> makeDigitMap()
	{
		StaticArray<char, kDigitMapCount> digitMap;
		for (size_t idx = 0; idx < kDigitMapHalfCount; ++idx)
		{
			digitMap[2 * idx + 0] = '0' + static_cast<char>(idx / 10);
			digitMap[2 * idx + 1] = '0' + static_cast<char>(idx % 10);
		}
		return digitMap;
	}

	constexpr StaticArray<uint64, kPowerOf10Count> makePowerOf10Map()
	{
		StaticArray<uint64, kPowerOf10Count> power10Map;
		power10Map[0] = 1;
		for (uint32 idx = 1; idx < kPowerOf10Count; ++idx)
		{
			power10Map[idx] = power10Map[idx - 1] * 10ull;
		}
		return power10Map;
	}

	static void extractSign(const char*& str, bool& isNegative)
	{
		if (str == nullptr)
		{
			isNegative = false;
			return;
		}
		isNegative = false;
		if (*str == '-')
		{
			isNegative = true;
			++str;
		}
		else if (*str == '+')
		{
			++str;
		}
	}

	static void extractDigits(const char*& str, uint32& result, size_t& digitCount)
	{
		if (str == nullptr)
		{
			result = 0;
			digitCount = 0;
			return;
		}

		result = 0;
		while (StrUtil::isDigit(*str))
		{
			result = result * 10 + (*str - '0');
			digitCount++;
			++str;
		}
		return;
	}

	int StrUtil::strToInt(const char* str)
	{
		bool isNegative = false;
		uint32 result = 0;
		size_t digitCount = 0;
		extractSign(str, isNegative);
		extractDigits(str, result, digitCount);
		return isNegative ? -static_cast<int>(result) : static_cast<int>(result);
	}

	float StrUtil::strToFloat(const char* str)
	{
		float result = 0.f;

		bool isNegative = false;
		uint32 integerPart = 0;
		size_t digitCount = 0;
		extractSign(str, isNegative);
		extractDigits(str, integerPart, digitCount);

		result = static_cast<float>(integerPart);

		if (*str == '.')
		{
			++str;

			uint32 fractionPart = 0;
			size_t fractionCount = 0;

			extractDigits(str, fractionPart, fractionCount);
			if (fractionCount > 0)
			{
				if (fractionCount >= kPowerOf10Count)
				{
					fractionCount = kPowerOf10Count - 1;
				}
				constexpr StaticArray<uint64, kPowerOf10Count> kPower10Map = makePowerOf10Map();
				result += fractionPart / static_cast<float>(kPower10Map[fractionCount]);
			}
		}

		return isNegative ? -result : result;
	}

	size_t getDigitLength(uint64 value)
	{
		if (value == 0) 
			return 1;

		constexpr StaticArray<uint64, kPowerOf10Count> power10Map = makePowerOf10Map();

		size_t low = 0;
		size_t high = kPowerOf10Count;
		while (low < high)
		{
			size_t mid = (low + high) >> 1;
			if (power10Map[mid] <= value) low = mid + 1;
			else high = mid;
		}
		return low;
	}

	void StrUtil::digitToStr(bool isNegative, uint64 value, IBuffer* buffer)
	{
		constexpr StaticArray<char, kDigitMapCount> digitMap = makeDigitMap();
		constexpr StaticArray<uint64, kPowerOf10Count> power10Map = makePowerOf10Map();
		constexpr char negative = '-';

		if (isNegative) buffer->writeBytes(&negative, 1);

		if (value == 0)
		{
			constexpr char zero = '0';
			buffer->writeBytes(&zero, 1);
		}
		else
		{
			size_t decimalLength = getDigitLength(value);
			const bool isLenghtOdd = decimalLength & 1;
			if (isLenghtOdd)
			{
				uint64 pow = power10Map[decimalLength - 1];
				uint64 leadingDigit = value / pow;
				buffer->writeBytes(&leadingDigit, 1);
				value -= pow * leadingDigit;
				--decimalLength;
			}

			while (decimalLength > 0)
			{
				uint64_t pow = power10Map[decimalLength - 2];
				uint64_t digits = value / pow;
				buffer->writeBytes(&digitMap[2 * digits], 2);
				value -= pow * digits;
				decimalLength -= 2;
			}
		}
	}

	void StrUtil::intToStr(bool isNegative, uint64 value, IBuffer* buffer)
	{
		digitToStr(isNegative, value, buffer);
	}

	void StrUtil::floatToStr(float value, IBuffer* buffer, size_t precision)
	{
		bool isNegative = value > 0;
		if (isnan(value)) return buffer->writeBytes(isNegative ? "-nan" : "nan", isNegative ? 4 : 3);
		if (isinf(value)) return buffer->writeBytes(isNegative ? "-inf" : "inf", isNegative ? 4 : 3);

		constexpr StaticArray<uint64, kPowerOf10Count> power10Map = makePowerOf10Map();
		precision = MathUtil::clamp<size_t>(precision, 1, 18);
		double ip;
		double frac = modf(fabs(value), &ip);
		uint64 intpart = static_cast<uint64>(ip);
		uint64_t pow10 = power10Map[precision];
		uint64 fracAsInteger = static_cast<uint64>(frac * static_cast<double>(pow10) + 0.5);
		if (fracAsInteger >= pow10)
		{
			fracAsInteger = 0;
			++intpart;
		}

		digitToStr(isNegative, intpart, buffer);
		constexpr char floatingPoint = '.';
		buffer->writeBytes(&floatingPoint, 1);
		digitToStr(false, fracAsInteger, buffer);
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