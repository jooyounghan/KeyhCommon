#include "CommonBasePch.h"
#include "FileUtil.h"

#if defined(KEYH_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(KEYH_PLATFORM_POSIX)
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace keyh
{
	namespace
	{
		Vector<StaticStringA> getEntryList(const char* dirPath, bool directory)
		{
			Vector<StaticStringA> entries;
			if (dirPath == nullptr)
			{
				return entries;
			}

#if defined(KEYH_PLATFORM_WINDOWS)
			char searchPath[MAX_PATH];
			const size_t dirPathLength = strlen(dirPath);
			const char* separator = (dirPathLength > 0 && (dirPath[dirPathLength - 1] == '\\' || dirPath[dirPathLength - 1] == '/')) ? "*" : "\\*";
			if (dirPathLength + strlen(separator) >= sizeof(searchPath))
			{
				return entries;
			}
			if (sprintf_s(searchPath, sizeof(searchPath), "%s%s", dirPath, separator) < 0)
			{
				return entries;
			}

			WIN32_FIND_DATAA findData;
			HANDLE handle = FindFirstFileA(searchPath, &findData);
			if (handle == INVALID_HANDLE_VALUE)
			{
				return entries;
			}

			do
			{
				const bool isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				if (isDirectory == directory && strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
				{
					entries.emplace_back(findData.cFileName);
				}
			} while (FindNextFileA(handle, &findData));

			FindClose(handle);
#elif defined(KEYH_PLATFORM_POSIX)
			DIR* directoryHandle = opendir(dirPath);
			if (directoryHandle == nullptr)
			{
				return entries;
			}

			while (dirent* entry = readdir(directoryHandle))
			{
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				{
					continue;
				}

				char entryPath[PATH_MAX];
				const int result = snprintf(entryPath, sizeof(entryPath), "%s%s%s", dirPath, dirPath[0] != '\0' && dirPath[strlen(dirPath) - 1] == '/' ? "" : "/", entry->d_name);
				if (result < 0 || result >= static_cast<int>(sizeof(entryPath)))
				{
					continue;
				}

				struct stat st;
				if (stat(entryPath, &st) == 0 && ((S_ISDIR(st.st_mode) != 0) == directory))
				{
					entries.emplace_back(entry->d_name);
				}
			}

			closedir(directoryHandle);
#endif
			return entries;
		}
	}

	bool FileUtil::isFileExist(const char* filePath)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		DWORD attributes = GetFileAttributesA(filePath);
		return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(KEYH_PLATFORM_POSIX)
		struct stat st;
		if (stat(filePath, &st) == 0)
		{
			return S_ISREG(st.st_mode);
		}
		return false;
#else
#endif
		return false;
	}
	
	bool FileUtil::isDirectoryExist(const char* dirPath)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		DWORD attributes = GetFileAttributesA(dirPath);
		return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(KEYH_PLATFORM_POSIX)
		struct stat st;
		if (stat(dirPath, &st) == 0)
		{
			return S_ISDIR(st.st_mode);
		}
		return false;
#else
#endif
		return false;
	}

	bool FileUtil::createDirectory(const char* dirPath)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		if (CreateDirectoryA(dirPath, nullptr))
		{
			return true;
		}
		return (GetLastError() == ERROR_ALREADY_EXISTS);
#elif defined(KEYH_PLATFORM_POSIX)
		if (mkdir(dirPath, 0755) == 0)
		{
			return true;
		}
		return (errno == EEXIST);
#else
		return false;
#endif
	}

	Vector<StaticStringA> FileUtil::getDirectoryList(const char* dirPath)
	{
		return getEntryList(dirPath, true);
	}

	Vector<StaticStringA> FileUtil::getFileList(const char* dirPath)
	{
		return getEntryList(dirPath, false);
	}

	uint64 FileUtil::getFileTimeStamp(const char* filePath)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		WIN32_FILE_ATTRIBUTE_DATA fileInfo;
		if (GetFileAttributesExA(filePath, GetFileExInfoStandard, &fileInfo))
		{
			ULARGE_INTEGER ull;
			ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
			ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;
			return ull.QuadPart;
		}
		return 0;
#elif defined(KEYH_PLATFORM_POSIX)
		struct stat st;
		if (stat(filePath, &st) == 0)
		{
			return static_cast<uint64>(st.st_mtime);
		}
		return 0;
#else
		return 0;
#endif
	}
}
