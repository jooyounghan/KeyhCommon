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

	template<typename Callback>
	void enumerateEntries(const char* dirPath, const char* searchPattern, Callback&& callback)
	{
		if (dirPath == nullptr)
		{
			return;
		}
#if defined(KEYH_PLATFORM_WINDOWS)
		char searchPath[MAX_PATH];
		const size_t dirPathLength = strlen(dirPath);
		const bool hasTrailingSlash = dirPathLength > 0 && (dirPath[dirPathLength - 1] == '\\' || dirPath[dirPathLength - 1] == '/');
		const char* pattern = (searchPattern != nullptr && searchPattern[0] != '\0') ? searchPattern : "*";

		if (sprintf_s(searchPath, sizeof(searchPath), hasTrailingSlash ? "%s%s" : "%s\\%s", dirPath, pattern) < 0)
		{
			return;
		}

		WIN32_FIND_DATAA findData;
		HANDLE handle = FindFirstFileA(searchPath, &findData);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return;
		}

		do
		{
			if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
			{
				continue;
			}

			const bool isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			callback(findData.cFileName, isDirectory ? EntryType::Directory : EntryType::File);
		} while (FindNextFileA(handle, &findData));

		FindClose(handle);
#elif defined(KEYH_PLATFORM_POSIX)
		DIR* directoryHandle = opendir(dirPath);
		if (directoryHandle == nullptr)
		{
			return;
		}

		while (dirent* entry = readdir(directoryHandle))
		{
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			{
				continue;
			}

			EntryType type = EntryType::File;
			if (entry->d_type == DT_DIR)
			{
				type = EntryType::Directory;
			}
			else if (entry->d_type == DT_UNKNOWN)
			{
				char entryPath[PATH_MAX];
				const int result = snprintf(entryPath, sizeof(entryPath), "%s%s%s", dirPath, dirPath[0] != '\0' && dirPath[strlen(dirPath) - 1] == '\\' ? "" : "\\", entry->d_name);
				if (result < 0 || result >= static_cast<int>(sizeof(entryPath)))
				{
					continue;
				}

				struct stat st;
				if (stat(entryPath, &st) == 0 && S_ISDIR(st.st_mode))
				{
					type = EntryType::Directory;
				}
			}

			callback(entry->d_name, type);
		}

		closedir(directoryHandle);
#endif
	}

	Vector<StaticStringA> FileUtil::getDirectoryList(const char* dirPath)
	{
		Vector<StaticStringA> entries;
		enumerateEntries(dirPath, "*", [&entries](const char* name, EntryType type)
			{
				if (type == EntryType::Directory)
				{
					entries.emplace_back(name);
				}
			});
		return entries;
	}

	Vector<StaticStringA> FileUtil::getFileList(const char* dirPath, const char* extension)
	{
		Vector<StaticStringA> entries;

		char patternBuffer[32];
		const char* pattern = "*";
		const char* targetExt = extension;

		if (targetExt != nullptr && targetExt[0] != '\0')
		{
			if (targetExt[0] == '.')
			{
				++targetExt;
			}
			sprintf_s(patternBuffer, sizeof(patternBuffer), "*.%s", targetExt);
			pattern = patternBuffer;
		}

		enumerateEntries(dirPath, pattern, [&entries, targetExt](const char* name, EntryType type)
			{
				if (type != EntryType::File)
				{
					return;
				}
#if defined(KEYH_PLATFORM_POSIX)
				if (targetExt != nullptr && targetExt[0] != '\0')
				{
					const char* dot = strrchr(name, '.');
					if (dot == nullptr || strcasecmp(dot + 1, targetExt) != 0)
					{
						return;
					}
				}
#endif
				entries.emplace_back(name);
			});

		return entries;
	}

	StaticStringA FileUtil::getFileStem(const StaticStringA& fileName)
	{
		const utf8* fileNameBuffer = fileName.c_str();
		const char* dot = StrUtil::findNext(fileNameBuffer, fileNameBuffer + fileName.length(), '.');

		dot = (dot != nullptr) ? dot : fileNameBuffer + fileName.length();
		StaticStringA stem(fileNameBuffer, static_cast<size_t>(dot - fileNameBuffer));
		return stem;
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
