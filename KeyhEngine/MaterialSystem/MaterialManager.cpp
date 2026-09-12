#include "MaterialSystemPch.h"
#include "MaterialManager.h"

#ifdef KEYH_DEV
#include "MaterialDefinition.h"
#include "MaterialParameterDefinition.h"
#endif

#include "MaterialLayout.h"
#include "MaterialParameterView.h"

#include <string>

namespace keyh
{
	static const StaticStringA& addMaterialDirectoryPathsFromFolder(const StaticStringA& folderPath, Vector<StaticStringA>& materialDirectoryPaths)
	{
		constexpr const utf8	kMaterialFolderPath[] = "\\Material";
		constexpr size_t		kMaterialFolderPathLength = sizeof(kMaterialFolderPath) - 1;

		StaticBufferA<kMaxPathLength> materialDirectoryPath;
		materialDirectoryPath.write(folderPath.c_str(), folderPath.size());
		materialDirectoryPath.write(kMaterialFolderPath, kMaterialFolderPathLength);

		return materialDirectoryPaths.emplace_back(materialDirectoryPath.getBuffer(), materialDirectoryPath.size());
	}

	static StaticStringA getMaterialBinaryPath(const StaticStringA& materialDirectoryPath)
	{
		constexpr const utf8	kMaterialBinaryPath[] = "\\Binary";
		constexpr size_t		kMaterialBinaryPathLength = sizeof(kMaterialBinaryPath) - 1;
		StaticBufferA<kMaxPathLength> materialBinaryFilePath;
		materialBinaryFilePath.write(materialDirectoryPath.c_str(), materialDirectoryPath.size());
		materialBinaryFilePath.write(kMaterialBinaryPath, kMaterialBinaryPathLength);
		return StaticStringA(materialBinaryFilePath.getBuffer(), materialBinaryFilePath.size());
	}

	MaterialManager::MaterialManager()
	{
		ResourcePathManager& resourcePathManager = ResourcePathManager::getInstance();
		const StaticStringA& commonResourcePath = resourcePathManager.getCommonResourcePath();
		addMaterialDirectoryPathsFromFolder(commonResourcePath, _materialDirectoryPaths);

		const StaticStringA& projectResourcePath = resourcePathManager.getProjectResourcePath();
		addMaterialDirectoryPathsFromFolder(projectResourcePath, _materialDirectoryPaths);

		importMaterialLayout(_materialDirectoryPaths);

#if defined(KEYH_DEV)
		// DEV일 때는 XML을 통해서 
		importMaterialDefinitions(_materialDirectoryPaths);
#endif
	}

	MaterialManager::~MaterialManager()
	{
	}

	void MaterialManager::importMaterialLayout(const Vector<StaticStringA>& directoryPaths)
	{
		for (const StaticStringA& directoryPath : directoryPaths)
		{
			const StaticStringA materialBinaryPath = getMaterialBinaryPath(directoryPath);

			Vector<StaticStringA> materialBinaryFiles = FileUtil::getFileList(materialBinaryPath.c_str(), "kem");
			for (const StaticStringA& materialBinaryFile : materialBinaryFiles)
			{
				StaticBufferA<kMaxPathLength> materialBinaryFilePath;
				materialBinaryFilePath.write(materialBinaryPath.c_str(), materialBinaryPath.size());
				materialBinaryFilePath.write("\\", 1);
				materialBinaryFilePath.write(materialBinaryFile.c_str(), materialBinaryFile.size());

				StaticStringA fileStem = FileUtil::getFileStem(materialBinaryFile);

				MaterialLayoutMap::FindResult findResult = _materialLayouts.find(fileStem);
				if (findResult.isFound() == true)
				{
					KEYH_ASSERT_DEV_ARGS(false, "MaterialLayout already exists for file: %s", materialBinaryFilePath.getBuffer());
					continue;
				}

				Ptr<MaterialLayout> materialLayout = makePtr<MaterialLayout>();

				StringViewA materialBinaryFilePathView(materialBinaryFilePath.getBuffer(), materialBinaryFilePath.size());
				ReflectSerializer::deserializeFromJson(materialBinaryFilePathView, materialLayout.get());

				_materialLayouts.insert(fileStem, keyh::move(materialLayout));
			}
		}
	}

#if defined KEYH_DEV
	void MaterialManager::importMaterialDefinitions(const Vector<StaticStringA>& directoryPaths)
	{
		for (const StaticStringA& directoryPath : directoryPaths)
		{
			const StaticStringA materialBinaryPath = getMaterialBinaryPath(directoryPath);

			const Vector<FileEntry> rebuildMateiralFileEntries = FileUtil::collectRebuildFileEntry(directoryPath.c_str(), "material", materialBinaryPath.c_str(), "kem");

			for (const FileEntry& materialFileEntry : rebuildMateiralFileEntries)
			{
				const StaticStringA& materialFilePath = materialFileEntry._fileFullPath;
				const StaticStringA& materialFileStem = materialFileEntry._fileStem;

				MaterialDefinition materialDefinition;

				StringViewA materialFilePathView(materialFilePath.c_str(), materialFilePath.size());
				ReflectSerializer::deserializeFromJson(materialFilePathView, &materialDefinition);

				materialDefinition.initialize();

				Ptr<MaterialLayout> materialLayout = makePtr<MaterialLayout>();
				materialDefinition.initializeMaterialLayout(materialLayout.get());

				// Need To Export MaterialLayout To Binary File

				_materialLayouts.insert(materialFileStem, keyh::move(materialLayout));
			}
		}
	}
#endif


}
