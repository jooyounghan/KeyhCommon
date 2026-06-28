#pragma once

namespace keyh
{
	struct FileUtil
	{
		static bool isFileExist(const char* filePath);
		static bool isDirectoryExist(const char* dirPath);
		static bool createDirectory(const char* dirPath);
	};

}