#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include "Delegate.h"
#include "AssertUtil.h"
#include "HashMap.h"

using namespace keyh;

int main()
{
	AssertUtil::gAssertHandler = keyh::move(AssertDelegate().bind([](const char* message) -> bool {
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

    while (true)
    {
	    HashMap<int, std::string> map;
	    map.insert(1, "one");
        map.find(1);
	    map.remove(1);
        map.reserve(100);
        map.clear();
    }
	for (size_t i = 0; i < HashUtil::kHashUtilCapacityTableSize; ++i)
	{
		std::cout << "Capacity for size " << i << ": " << HashUtil::getHashUtilCapacityTable()[i] << " expected: " << MathUtil::nextPrime(HashUtil::kInitialCapacity * (1ULL << i)) << std::endl;

	}

    StaticArray<int32, HashUtil::kHashUtilCapacityTableSize> capacityTable = HashUtil::getHashUtilCapacityTable();
    return 0;
}