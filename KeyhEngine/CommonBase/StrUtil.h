#pragma once
#include "AssertUtil.h"
#include "StaticArray.h"

namespace keyh
{
	class IBuffer;

	struct StrUtil
	{
		static constexpr size_t kSsoCapacity = 16;

#pragma region String Length and Compare
		template<typename T>
		static size_t strlen(const T* str);	

		template<typename T>
		static int strcmp(const T* lhs, const T* rhs);
#pragma endregion

#pragma region Convert
		static int strToInt(const char* str);
		static float strToFloat(const char* str);

	private:
		static void digitToStr(bool isNegative, uint64 value, IBuffer* buffer);
		
	public:
		static void intToStr(bool isNegative, uint64 value, IBuffer* buffer);
		static void floatToStr(float value, IBuffer* buffer, size_t precision = 3);
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

