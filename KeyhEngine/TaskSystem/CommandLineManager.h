#pragma once

namespace keyh
{
    class CommandLineManager
	{
        SINGLETON(CommandLineManager);

    private:
        template<typename Target>
		using Hasher = FNV1aHash<Target>;

        HashSet<StaticStringA, Hasher<StaticStringA>>                  _commandLineSet;
		HashMap<StaticStringA, StaticStringA, Hasher<StaticStringA>>   _commandLineWithArgMap;

    public:
		bool isCommandLinePresent(const StringViewA& command) const;
	};
}

