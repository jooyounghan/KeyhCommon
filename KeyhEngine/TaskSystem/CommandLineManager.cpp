#include "TaskSystemPch.h"
#include "CommandLineManager.h"

namespace keyh
{
	bool CommandLineManager::isCommandLinePresent(const StaticStringA& command) const
	{
		return _commandLineSet.find(command).isFound()
			|| _commandLineWithArgMap.find(command).isFound();
	}

	const StaticStringA& CommandLineManager::getCommandLineArgument(const StaticStringA& command) const
	{
		CommandLineWithArgMap::ConstFindResult result = _commandLineWithArgMap.find(command);
		if (result.isFound())
			return *result.value();

		return StaticStringA::Empty;
	}

	void CommandLineManager::registerCommandLines(int argc, char* argv[])
	{
        for (int i = 0; i < argc; ++i)
        {
			SplitterStringA splitter(argv[i], "=");
            const Vector<StringViewA>& tokens = splitter.getTokens();
            if (tokens.size() == 1)
            {
				const StringViewA& token = tokens[0];
                _commandLineSet.insert(StaticStringA(token.c_str(), token.length()));
            }
            else if (tokens.size() == 2)
            {
                _commandLineWithArgMap.insert(
					StaticStringA(tokens[0].c_str(), tokens[0].length())
					, StaticStringA(tokens[1].c_str(), tokens[1].length())
				);
            }
        }
	}
}