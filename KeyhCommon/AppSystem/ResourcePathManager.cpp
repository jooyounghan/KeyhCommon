#include "AppSystemPch.h"
#include "ResourcePathManager.h"
#include "CommandLineManager.h"

namespace keyh
{
	KEYH_REFLECT_DEFINE_BODY(ProjectNode);

	void ResourcePathManager::initializeResourcePaths()
	{
		CommandLineManager& commandLineManager = CommandLineManager::getInstance();

		if (commandLineManager.isCommandLinesRegistered() == false)
		{
			KEYH_ASSERT_DEV(false, "Command lines are not registered. Please call CommandLineManager::registerCommandLines() before initializing resource paths.");
			return;
		}

		for (uint8 index = 0; index < kProjectPathCount; ++index)
		{
			const EProjectPath projectPath = static_cast<EProjectPath>(index);
			const char* projectPathName = ReflectEnumTraits<EProjectPath>::toString(projectPath);
			StaticBufferA<kMaxPathLength> argumentName;
			argumentName.format("%sResourcePath", projectPathName);
			_projectPaths[index] = commandLineManager.getCommandLineArgument(StaticStringA(argumentName.getBuffer(), argumentName.size()));
		}

		const StaticStringA configurationPath = commandLineManager.getCommandLineArgument("ResourcePathConfig");
		if (configurationPath.empty())
		{
			KEYH_ASSERT_DEV(false, "ResourcePathConfig command line argument is not provided. Please provide the project path graph configuration file.");
			return;
		}

		ReflectSerializer::deserializeFromJson(StringViewA(configurationPath.c_str(), configurationPath.size()), &_rootNode);

		_orderedResourcePaths.clear();
		appendPathsInReverseTopologicalOrder(_rootNode);
	}

	bool ResourcePathManager::appendPathsInReverseTopologicalOrder(const ProjectNode& projectNode)
	{
		for (const ProjectNode& childNode : projectNode.getChildren())
		{
			if (appendPathsInReverseTopologicalOrder(childNode) == false)
				return false;
		}

		const uint8 index = static_cast<uint8>(projectNode.getProjectPath());
		if (index >= kProjectPathCount)
		{
			KEYH_ASSERT_DEV(false, "Project path graph contains an invalid project path enum value.");
			return false;
		}
		if (_projectPaths[index].empty())
		{
			if (index != static_cast<uint8>(EProjectPath::Common))
			{
				KEYH_ASSERT_DEV(false, "Project path graph references a resource path that was not provided on the command line.");
				return false;
			}
			return true;
		}

		_orderedResourcePaths.push_back(StringViewA(_projectPaths[index].c_str(), _projectPaths[index].size()));
		return true;
	}
}

#include "ResourcePathManager.reflect_generated.cpp.inl"
