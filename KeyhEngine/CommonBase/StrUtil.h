#pragma once
#include <cstring>
#include <cwchar>
#include "AssertUtil.h"
namespace keyh
{
	struct StrUtil
	{
		static constexpr size_t ssoCapacity = 16;

#pragma region String Length and Compare
		template<typename T>
		static size_t strlen(const T* str);	

		template<typename T>
		static int strcmp(const T* lhs, const T* rhs);
#pragma endregion

#pragma region Convert
		static int strToInt(const char* str, int radix = 10, char** endPtr = nullptr);
		static float strToFloat(const char* str, char** endPtr = nullptr);
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

