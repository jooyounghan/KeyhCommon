#include "MaterialSystemPch.h"
#include "MaterialManager.h"

namespace keyh
{
	static const StaticStringA& addMaterialPathsFromFolder(const StaticStringA& folderPath, Vector<StaticStringA>& materialPaths)
	{
		constexpr const utf8	kMaterialFolderPath[] = "/Material";
		constexpr size_t		kMaterialFolderPathLength = sizeof(kMaterialFolderPath);

		StaticBufferA<kMaxPathLength> materialPath;
		materialPath.write(folderPath.c_str(), folderPath.size());
		materialPath.write(kMaterialFolderPath, kMaterialFolderPathLength);

		return materialPaths.emplace_back(materialPath.getBuffer(), materialPath.size());
	}

	MaterialManager::MaterialManager()
	{
		ResourcePathManager& resourcePathManager = ResourcePathManager::getInstance();
		const StaticStringA& commonResourcePath = resourcePathManager.getCommonResourcePath();
		const StaticStringA& commonMaterialPath = addMaterialPathsFromFolder(commonResourcePath, _materialPaths);

		Vector<StaticStringA> commonMaterialFiles = FileUtil::getFileList(commonMaterialPath.c_str(), "material");


		const StaticStringA& projectResourcePath = resourcePathManager.getProjectResourcePath();
		const StaticStringA& projectMaterialPath = addMaterialPathsFromFolder(projectResourcePath, _materialPaths);
	}

	MaterialManager::~MaterialManager()
	{
	}

}
