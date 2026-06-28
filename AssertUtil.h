#pragma once
#include <cstdlib>
#include <cstdio>

#include "Delegate.h"

#if defined(_MSC_VER)
#define KEYH_DEBUG_BREAK() __debugbreak()
#else
#define KEYH_DEBUG_BREAK() __builtin_trap()
#endif

#if defined(__clang__)
#define FUNC_NAME __PRETTY_FUNCTION__
#elif defined(__GNUC__)
#define FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC_NAME __FUNCSIG__
#else
#define FUNC_NAME __func__
#endif

#define STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(ClassName)	\
	static_assert(false,								\
		#ClassName										\
		" does not support for this template type."		\
		" Triggered in: " FUNC_NAME						\
	)

namespace keyh
{
	DECLARE_DELEGATE(AssertDelegate, bool, const char*);

#define KEYH_ASSERT(condition, message) AssertUtil::check(condition, message)
#define KEYH_ASSERT_ARGS(condition, format, ...) AssertUtil::check(condition, format, ##__VA_ARGS__)

#ifdef KEYH_DEV
#define KEYH_ASSERT_DEV(condition, message)				KEYH_ASSERT(condition, message)
#define KEYH_ASSERT_DEV_ARGS(condition, format, ...)	KEYH_ASSERT_ARGS(condition, format, __VA_ARGS__)
#else
#define KEYH_ASSERT_DEV(condition, message)				KEYH_NOOP()
#define KEYH_ASSERT_DEV_ARGS(condition, format, ...)	KEYH_NOOP()
#endif // KEYH_DEV

	struct AssertUtil
	{
		static AssertDelegate gAssertHandler;
		static void check(bool condition, const char* message = "Assertion failed") noexcept;

		template <typename... Args>
		static void check(bool condition, const char* format, Args&&... args) noexcept;
	};

}
#include "AssertUtil.hpp"
