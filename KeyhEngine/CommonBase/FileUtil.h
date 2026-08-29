#pragma once
#include "CommonCore.h"
#include "StaticString.h"
#include "Vector.h"
namespace keyh
{
	struct FileUtil
	{
		static bool isFileExist(const char* filePath);
		static bool isDirectoryExist(const char* dirPath);
		static bool createDirectory(const char* dirPath);

		static Vector<StaticStringA> getDirectoryList(const char* dirPath);
		static Vector<StaticStringA> getFileList(const char* dirPath);

		static uint64 getFileTimeStamp(const char* filePath);
	};

}