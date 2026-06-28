#pragma once
#include "CommonCore.h"
namespace keyh
{
	struct FileUtil
	{
		static bool isFileExist(const char* filePath);
		static bool isDirectoryExist(const char* dirPath);
		static bool createDirectory(const char* dirPath);

		static uint64 getFileTimeStamp(const char* filePath);
	};

}