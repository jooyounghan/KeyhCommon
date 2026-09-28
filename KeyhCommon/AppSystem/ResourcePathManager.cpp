#include "AppSystemPch.h"
#include "ResourcePathManager.h"
#include "CommandLineManager.h"

namespace keyh
{
	void ResourcePathManager::initializeResourcePaths()
	{
		CommandLineManager& commandLineManager = CommandLineManager::getInstance();

		if (commandLineManager.isCommandLinesRegistered() == false)
		{
			KEYH_ASSERT_DEV(false, "Command lines are not registered. Please call CommandLineManager::registerCommandLines() before initializing resource paths.");
			return;
		}

		_commonResourcePath = commandLineManager.getCommandLineArgument("CommonResourcePath");
		if (_commonResourcePath.empty())
		{
			KEYH_ASSERT_DEV(false, "CommonResourcePath command line argument is not provided. Please provide a valid path.");
			return;
		}

		_projectResourcePath = commandLineManager.getCommandLineArgument("ProjectResourcePath");
		if (_projectResourcePath.empty())
		{
			KEYH_ASSERT_DEV(false, "ProjectResourcePath command line argument is not provided. Please provide a valid path.");
			return;
		}
	}
}
