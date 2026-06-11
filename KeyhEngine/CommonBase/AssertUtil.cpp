#include "CommonBasePch.h"
#include "AssertUtil.h"
#include "TypeCommon.h"

namespace keyh
{
	AssertDelegate AssertUtil::gAssertHandler = keyh::move(AssertDelegate().bind([](const char*) { return true; }));

	void AssertUtil::check(bool condition, const char* message) noexcept
	{
		if (condition == true)
			return;

		if (gAssertHandler.isValid() && gAssertHandler(message))
		{
			__debugbreak();
		}
		else
		{
			std::abort();
		}
	}
}