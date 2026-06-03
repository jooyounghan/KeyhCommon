#include <crtdbg.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <cassert>
#include "Delegate.h"
#include "AssertUtil.h"
#include <Windows.h>
using namespace keyh;

int main()
{
	AssertUtil::gAssertHandler = std::move(AssertDelegate().bind([](const char* message) -> bool {
        int result = ::MessageBoxA(
            nullptr,
            message,
            "Assertion Failed!",
            MB_YESNO | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND
        );

        if (result == IDYES)
        {
            return true;
        }

        return false;
        })
    );

	KEYH_ASSERT_ARGS(1 + 1 != 2, "Math is broken: %d + %d != %d", 1, 1, 2);
    
	printf("This line will not be reached if the assertion fails and the user chooses to break into the debugger.\n");

    return 0;
}