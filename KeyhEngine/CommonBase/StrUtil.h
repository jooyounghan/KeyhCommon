#pragma once
#include "AssertUtil.h"
namespace keyh
{
	struct StrUtil
	{
		constexpr static size_t ssoCapacity = 16;

		template<typename T>
		static size_t strlen(const T* str);	

		template<typename T>
		static int strcmp(const T* lhs, const T* rhs);
	};
}
#include "StrUtil.hpp"

