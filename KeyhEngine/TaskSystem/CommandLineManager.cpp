#include "TaskSystemPch.h"
#include "CommandLineManager.h"

namespace keyh
{
	bool CommandLineManager::isCommandLinePresent(const StringViewA& command) const
	{
		Hasher<StringViewA> hasher;
		size_t hash = hasher(command);

		return false;
	}
}