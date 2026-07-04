#pragma once
#include <cstring>
#include <cwchar>
#include "AssertUtil.h"
#include "StaticArray.h"

namespace keyh
{
	class IBufferBase;

	struct StrUtil
	{
		static constexpr size_t kSsoCapacity = 16;
		static constexpr size_t kDigitMapCount = 200;
		static constexpr size_t kDigitMapHalfCount = kDigitMapCount / 2;
		static constexpr size_t kPowerOf10Count = 20;

#pragma region String Length and Compare
		template<typename T>
		static size_t strlen(const T* str);	

		template<typename T>
		static int strcmp(const T* lhs, const T* rhs);
#pragma endregion

#pragma region Convert
		static int strToInt(const char* str, int radix = 10, char** endPtr = nullptr);
		static float strToFloat(const char* str, char** endPtr = nullptr);

	private:
		constexpr StaticArray<char, kDigitMapCount> makeDigitMap();
		constexpr StaticArray<uint64, kPowerOf10Count> makePowerOf10Map();
		void digitToStr(bool isNegative, uint64 value, IBufferBase* buffer);

	public:
		static void intToStr(bool isNegative, uint64 value, IBufferBase* buffer);
		static void floatToStr(float value, IBufferBase* buffer, int precision = 3);
#pragma endregion

#pragma region String Search and Skips
		static const char* findNext(const char* start, const char* end, const char target);
		static const char* skipWhiteSpace(const char* start, const char* end);

		inline static bool isWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
		inline static bool isDigit(char c) { return '0' <= c && c <= '9'; }
#pragma endregion
	};
}
#include "StrUtil.hpp"

