#include "CommonBasePch.h"
#include "AssertUtil.h"

namespace keyh
{
	AssertDelegate AssertUtil::gAssertHandler = std::move(AssertDelegate().bind([](const char*) { return true; }));

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