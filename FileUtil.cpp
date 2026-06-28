#include "CommonBasePch.h"
#include "FileUtil.h"

#if defined(KEYH_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(KEYH_PLATFORM_POSIX)
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
