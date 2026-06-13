#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include "Delegate.h"
#include "AssertUtil.h"
#include "HashMap.h"

using namespace keyh;

int main()
{
	//AssertUtil::gAssertHandler = keyh::move(AssertDelegate().bind([](const char* message) -> bool {
 //       int result = ::MessageBoxA(
 //           nullptr,
 //           message,
 //           "Assertion Failed!",
 //           MB_YESNO | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND
 //       );

 //       if (result == IDYES)
 //       {
 //           return true;
 //       }

 //       return false;
 //       })
 //   );

	//KEYH_ASSERT_ARGS(1 + 1 != 2, "Math is broken: %d + %d != %d", 1, 1, 2);
 //   
	//printf("This line will not be reached if the assertion fails and the user chooses to break into the debugger.\n");

    while (true)
    {
	    HashMap<int, std::string> map;
	    map.insert(1, "one");
        map.insert(2, "two");
        map.insert(3, "three");
        map.insert(4, "four");
		map.insert(5, "five");
		map.insert(6, "six");
		map.insert(7, "seven");
		map.insert(8, "eight");
		map.insert(9, "nine");
		map.insert(10, "ten");



        map.find(1);
	    map.remove(1);
        map.reserve(100);
        map.clear();
    }
    return 0;
}