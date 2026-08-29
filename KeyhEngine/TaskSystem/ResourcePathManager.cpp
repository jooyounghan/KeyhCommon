#include "TaskSystemPch.h"
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
		_projectResourcePath = commandLineManager.getCommandLineArgument("ProjectResourcePath");
	}
}
