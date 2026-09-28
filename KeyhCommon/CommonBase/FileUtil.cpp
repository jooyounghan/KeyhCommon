#include "CommonBasePch.h"
#include "FileUtil.h"
#include "HashMap.h"

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

	static bool isPathSeparator(char ch)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		return ch == '/' || ch == '\\';
#else
		return ch == '/';
#endif
	}

	size_t getPathRootLength(const char* path, size_t length)
	{
		if (length == 0)
		{
			return 0;
		}

#if defined(KEYH_PLATFORM_WINDOWS)

		if (length >= 3 &&
			path[1] == ':' &&
			isPathSeparator(path[2]))
		{
			return 3;
		}

		if (length >= 2 &&
			isPathSeparator(path[0]) &&
			isPathSeparator(path[1]))
		{
			size_t index = 2;

			while (index < length && !isPathSeparator(path[index]))
			{
				++index;
			}

			if (index < length)
			{
				++index;
			}

			while (index < length && !isPathSeparator(path[index]))
			{
				++index;
			}

			if (index < length)
			{
				++index;
			}

			return index;
		}

#endif

		return isPathSeparator(path[0]) ? 1 : 0;
	}

	bool FileUtil::createDirectories(const char* dirPath)
	{
		if (dirPath == nullptr || *dirPath == '\0')
		{
			return false;
		}

		const size_t length = strlen(dirPath);
		if (length >= kMaxPathLength)
		{
			return false;
		}

		StaticBufferA<kMaxPathLength> pathBuffer;
		char* path = pathBuffer.getBuffer();

		memcpy(path, dirPath, length + 1);

		const size_t rootLength = getPathRootLength(path, length);

		for (size_t i = rootLength; i < length; ++i)
		{
			if (!isPathSeparator(path[i]))
			{
				continue;
			}

			if (i > rootLength && isPathSeparator(path[i - 1]))
			{
				continue;
			}

			const char separator = path[i];
			path[i] = '\0';

			if (!createDirectory(path))
			{
				return false;
			}

			path[i] = separator;
		}

		return createDirectory(path);
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

	Vector<FileEntry> FileUtil::getFileList(const char* dirPath, const char* extension)
	{
		Vector<FileEntry> entries;

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

		enumerateEntries(dirPath, pattern, [&entries, dirPath, targetExt](const char* name, EntryType type)
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
				const size_t nameLength = strlen(name);
				const char* dot = StrUtil::findNext(name, name + nameLength, '.');
				const char* extensionBegin = (dot != nullptr) ? dot + 1 : name + nameLength;

				StaticBufferA<kMaxPathLength> fullPath;
				fullPath.write(dirPath, strlen(dirPath));
				fullPath.write("\\", 1);
				fullPath.write(name, nameLength);

				FileEntry fileEntry;
				fileEntry._fileStem = StaticStringA(name, static_cast<size_t>(extensionBegin - name - (dot != nullptr ? 1 : 0)));
				fileEntry._fileExtension = StaticStringA(extensionBegin, static_cast<size_t>(name + nameLength - extensionBegin));
				fileEntry._fileFullPath = StaticStringA(fullPath.getBuffer(), fullPath.size());
				entries.push_back(keyh::move(fileEntry));
			});

		return entries;
	}

	struct FilePathEntry
	{
		StaticStringA _filePath;
		StaticStringA _binaryFilePath;
	};
	using FileNameMap = HashMap<StaticStringA, FilePathEntry>;

	static void updateFileNameMap(const utf8* path, const utf8* extension, FileNameMap& fileNameMap, bool isBinary)
	{
		Vector<FileEntry> files = FileUtil::getFileList(path, extension);
		for (const FileEntry& file : files)
		{
			FileNameMap::InsertResult insertResult = fileNameMap.insert(file._fileStem, FilePathEntry(), false);
			FilePathEntry& filePathEntry = insertResult.value();

			if (isBinary)
			{
				filePathEntry._binaryFilePath = file._fileFullPath;
			}
			else
			{
				filePathEntry._filePath = file._fileFullPath;
			}
		}
	}

	Vector<FileEntry> FileUtil::collectRebuildFileEntry(const char* rawPath, const char* rawExtension, const char* binaryPath, const char* binaryExtension, bool forceCollect)
	{
		Vector<FileEntry> rebuildFileList;

		HashMap<StaticStringA, FilePathEntry> fileNameMap;
		updateFileNameMap(rawPath, rawExtension, fileNameMap, false);
		updateFileNameMap(binaryPath, binaryExtension, fileNameMap, true);

		const StaticStringA extension(rawExtension, strlen(rawExtension));

		for (const HashBucket<StaticStringA, FilePathEntry>& bucket : fileNameMap)
		{
			const FilePathEntry& filePathEntry = bucket.value();
			const bool isRawFileExist = !filePathEntry._filePath.empty();
			const bool isRebuildNeeded = isRawFileExist && (filePathEntry._binaryFilePath.empty() || getFileTimeStamp(filePathEntry._filePath.c_str()) > getFileTimeStamp(filePathEntry._binaryFilePath.c_str()));

			if (isRebuildNeeded == false && forceCollect == false)
				continue;

			const StaticStringA& fileStem = bucket.key();

			StaticBufferA<kMaxPathLength> rebuildFilePath;
			rebuildFilePath.write(rawPath, strlen(rawPath));
			rebuildFilePath.write("\\", 1);
			rebuildFilePath.write(fileStem.c_str(), fileStem.size());
			rebuildFilePath.write(".", 1);
			rebuildFilePath.write(extension.c_str(), extension.size());
			StaticStringA rebuildFile(rebuildFilePath.getBuffer(), rebuildFilePath.size());

			FileEntry fileEntry;
			fileEntry._fileStem = fileStem;
			fileEntry._fileExtension = extension;
			fileEntry._fileFullPath = rebuildFile;

			rebuildFileList.push_back(keyh::move(fileEntry));
		}

		return rebuildFileList;
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
