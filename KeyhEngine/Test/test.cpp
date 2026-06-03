#include <crtdbg.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <cassert>
#include "Delegate.h"

using namespace keyh;


int main()
{
	DECLARE_DELEGATE(TestDelegate, void, int);
	TestDelegate delegate;
	delegate.bind([](int value) { std::cout << "Value: " << value << std::endl; });

	delegate.invoke(42);
    return 0;
}