#pragma once
#include "Delegate.h"

namespace keyh
{
	DECLARE_DELEGATE(AssertDelegate, bool, const char*);

#define KEYH_ASSERT(condition, message) AssertUtil::check(condition, message)
#define KEYH_ASSERT_ARGS(condition, format, ...) AssertUtil::check(condition, format, ##__VA_ARGS__)

#ifdef KEYH_DEV
#define KEYH_ASSERT_DEV(condition, message)				KEYH_ASSERT(condition, message)
#define KEYH_ASSERT_DEV_ARGS(condition, format, ...)	KEYH_ASSERT_ARGS(condition, format, __VA_ARGS__)
#else
#define KEYH_ASSERT_DEV(condition, message)				__noop
#define KEYH_ASSERT_DEV_ARGS(condition, format, ...)	__noop
#endif // KE_DEV

#define KEYH_ASSERT_DEBUG_ARGS(condition, format, ...) AssertUtil::check(condition, format, ##__VA_ARGS__)

	struct AssertUtil
	{
		static AssertDelegate gAssertHandler;
		static void check(bool condition, const char* message = "Assertion failed") noexcept;

		template <typename... Args>
		static void check(bool condition, const char* format, Args&&... args) noexcept;
	};

}
#include "AssertUtil.hpp"
