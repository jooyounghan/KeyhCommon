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

	struct FileEntry
	{
		StaticStringA _fileStem;
		StaticStringA _fileExtension;
		StaticStringA _fileFullPath;
	};

	struct FileUtil
	{
		static bool isFileExist(const char* filePath);
		static bool isDirectoryExist(const char* dirPath);
		static bool createDirectory(const char* dirPath);

		static Vector<StaticStringA> getDirectoryList(const char* dirPath);
		static Vector<StaticStringA> getFileList(const char* dirPath, const char* extension = nullptr);

		static Vector<FileEntry> collectRebuildFileEntry(const char* rawPath, const char* rawExtension, const char* binaryPath, const char* binaryExtension, bool forceCollect);

		static StaticStringA getFileStem(const StaticStringA& fileName);

		static uint64 getFileTimeStamp(const char* filePath);
	};

}