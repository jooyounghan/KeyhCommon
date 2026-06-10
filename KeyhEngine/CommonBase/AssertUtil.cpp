#include "CommonBasePch.h"
#include "AssertUtil.h"
#include "TypeCommon.h"

namespace keyh
{
	AssertDelegate AssertUtil::gAssertHandler = move(AssertDelegate().bind([](const char*) { return true; }));

	void AssertUtil::check(bool condition, const char* message) noexcept
	{
		if (condition == false)
		{
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
}