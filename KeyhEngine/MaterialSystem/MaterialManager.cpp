#include "MaterialSystemPch.h"
#include "MaterialManager.h"

#ifdef PA_DEV
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

#if defined KEYH_DEV
	static const void importMaterialDefinitions(const Vector<StaticStringA>& directoryPaths, MaterialManager::MaterialLayoutMap& materialLayouts)
	{
		for (const StaticStringA& directoryPath : directoryPaths)
		{
			Vector<StaticStringA> materialFiles = FileUtil::getFileList(directoryPath.c_str(), "material");
			for (const StaticStringA& materialFile : materialFiles)
			{
				StaticBufferA<kMaxPathLength> materialFilePath;
				materialFilePath.write(directoryPath.c_str(), directoryPath.size());
				materialFilePath.write("\\", 1);
				materialFilePath.write(materialFile.c_str(), materialFile.size());

				StaticStringA fileStem = FileUtil::getFileStem(materialFile);
			}
		}
	}
#endif

	MaterialManager::MaterialManager()
	{
		ResourcePathManager& resourcePathManager = ResourcePathManager::getInstance();
		const StaticStringA& commonResourcePath = resourcePathManager.getCommonResourcePath();
		const StaticStringA& commonMaterialDirectoryPath = addMaterialDirectoryPathsFromFolder(commonResourcePath, _materialDirectoryPaths);

		const StaticStringA& projectResourcePath = resourcePathManager.getProjectResourcePath();
		const StaticStringA& projectMaterialDirectoryPath = addMaterialDirectoryPathsFromFolder(projectResourcePath, _materialDirectoryPaths);

#if defined(KEYH_DEV)
		// DEV일 때는 XML을 통해서 
		importMaterialDefinitions(_materialDirectoryPaths, _materialLayouts);
#endif

#pragma region Material Definition Create Test
//		MaterialDefinition test;
//		{
//			MaterialParameterDefinition* materialParameterInfo = test._materialParameterDefinitions.emplace_back();
//			materialParameterInfo->_parameterName = "TestBitFlagParameter";
//			materialParameterInfo->_parameterType = MaterialParameterType::BitFlag8;
//			materialParameterInfo->_description = "Test Bit Flag Parameter Description";
//			for (uint32 idx = 0; idx < 8; ++idx)
//			{
//				MaterialBitFlagDefinition& bitFlagInfo = materialParameterInfo->_bitFlagDefinitions.emplace_back();
//				bitFlagInfo._bitIndex = idx;
//				bitFlagInfo._parameterName = ("Bit Flag Name" + std::to_string(idx)).c_str();
//				bitFlagInfo._defaultValue = idx % 2 == 0;
//				bitFlagInfo._description = ("Bit Flag Description" + std::to_string(idx)).c_str();
//			}
//		}
//
//
//		for (uint32 idx = 0; idx < 3; ++idx)
//		{
//			MaterialParameterInfo* materialParameterInfo = test._materialParameterInfos.emplace_back();
//			materialParameterInfo->_parameterName = "TestParameter";
//			materialParameterInfo->_parameterType = MaterialParameterType::Float;
//			materialParameterInfo->_description = "Test Parameter Description";
//			materialParameterInfo->_defaultValue = "0.0";
//		}
//
//		StaticBufferA<kMaxPathLength> materialFilePath;
//		materialFilePath.write(commonMaterialDirectoryPath.c_str(), commonMaterialDirectoryPath.size());
//		materialFilePath.write("\\TestMaterial.material", sizeof("\\TestMaterial.material") - 1);
//
//		StringViewA materialFilePathView(materialFilePath.getBuffer(), materialFilePath.size());
//	
//		ReflectSerializer::serializeToJson(materialFilePathView, &test, true);
#pragma endregion

	}

	MaterialManager::~MaterialManager()
	{
	}

}
