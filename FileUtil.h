#pragma once
#include "CommonCore.h"
#include "StaticString.h"
#include "Vector.h"
namespace keyh
{
	enum class EntryType
	{
		Directory,
		File
	};

	struct FileUtil
	{
		static bool isFileExist(const char* filePath);
		static bool isDirectoryExist(const char* dirPath);
		static bool createDirectory(const char* dirPath);

		static Vector<StaticStringA> getDirectoryList(const char* dirPath);
		static Vector<StaticStringA> getFileList(const char* dirPath, const char* extension = nullptr);

		static StaticStringA getFileStem(const StaticStringA& fileName);

		static uint64 getFileTimeStamp(const char* filePath);
	};

}