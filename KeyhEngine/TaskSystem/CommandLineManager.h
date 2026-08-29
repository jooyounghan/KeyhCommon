#pragma once

namespace keyh
{
    class CommandLineManager
	{
        SINGLETON(CommandLineManager);

    private:
        template<typename Target>
		using Hasher = FNV1aHash<Target>;

        using CommandLineSet = HashSet<StaticStringA, Hasher<StaticStringA>>;
		using CommandLineWithArgMap = HashMap<StaticStringA, StaticStringA, Hasher<StaticStringA>>;

        CommandLineSet                  _commandLineSet;
		CommandLineWithArgMap           _commandLineWithArgMap;

    public:		
        bool                    isCommandLinePresent(const StaticStringA& command) const;
		const StaticStringA&    getCommandLineArgument(const StaticStringA& command) const;

    public:
		inline bool 			isCommandLinesRegistered() const { return _commandLineSet.size() > 0 || _commandLineWithArgMap.size() > 0; }

    public:
		void registerCommandLines(int argc, char* argv[]);
	};
}

