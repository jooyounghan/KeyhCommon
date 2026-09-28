#include "CommonBasePch.h"
#include "File.h"
#if defined(KEYH_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(KEYH_PLATFORM_POSIX)
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace keyh
{
	File::~File() { unload(); }

	bool File::load(const char* filePath)
	{
		unload();
		if (filePath == nullptr)
			return false;
#if defined(KEYH_PLATFORM_WINDOWS)
		_fileHandle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
#elif defined(KEYH_PLATFORM_POSIX)
		_fileDescriptor = open(filePath, O_RDONLY);
#endif
		if (mapOpenedFile())
			return true;
		unload();
		return false;
	}

	bool File::load(const wchar_t* filePath)
	{
		unload();
		if (filePath == nullptr)
			return false;
#if defined(KEYH_PLATFORM_WINDOWS)
		_fileHandle = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (mapOpenedFile())
			return true;
		unload();
		return false;
#elif defined(KEYH_PLATFORM_POSIX)
		const size_t length = StrUtil::strlen(filePath);
		if (length > (kInvalidSizeT - 1) / MB_CUR_MAX)
			return false;
		DynamicBufferA path;
		path.allocate(length * MB_CUR_MAX + 1);
		if (::wcstombs(path.getBuffer(), filePath, path.capacity()) == static_cast<size_t>(-1))
			return false;
		return load(path.getBuffer());
#else
		return false;
#endif
	}

	bool File::mapOpenedFile()
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		if (_fileHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		LARGE_INTEGER size;
		if (!GetFileSizeEx(_fileHandle, &size))
		{
			return false;
		}
		if (size.QuadPart < 0 || static_cast<unsigned long long>(size.QuadPart) > SIZE_MAX)
			return false;
		_fileSize = static_cast<size_t>(size.QuadPart);
		if (_fileSize == 0)
			return false;

		const DWORD sizeHigh = static_cast<DWORD>(static_cast<unsigned long long>(size.QuadPart) >> 32);
		const DWORD sizeLow = static_cast<DWORD>(size.QuadPart & 0xFFFFFFFF);

		_mappingHandle = CreateFileMappingA(_fileHandle, nullptr, PAGE_WRITECOPY, sizeHigh, sizeLow, nullptr);
		if (_mappingHandle == nullptr)
		{
			return false;
		}

		_stringBuffer = static_cast<char*>(MapViewOfFile(_mappingHandle, FILE_MAP_COPY, 0, 0, _fileSize));
		return _stringBuffer != nullptr;

#elif defined(KEYH_PLATFORM_POSIX)
		if (_fileDescriptor == kInvalidFileDescriptor)
		{
			return false;
		}

		struct stat fileInfo;
		if (fstat(_fileDescriptor, &fileInfo) == -1)
		{
			return false;
		}
		_fileSize = static_cast<size_t>(fileInfo.st_size);
		if (_fileSize == 0)
			return false;

		void* mappedMemory = mmap(nullptr, _fileSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, _fileDescriptor, 0);
		if (mappedMemory == MAP_FAILED)
		{
			return false;
		}

		_stringBuffer = static_cast<char*>(mappedMemory);
		return true;
#else
		return false; // Unsupported platform
#endif
	}

	void File::unload()
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		if (_stringBuffer != nullptr)
		{
			UnmapViewOfFile(_stringBuffer);
			_stringBuffer = nullptr;
		}
		if (_mappingHandle != nullptr)
		{
			CloseHandle(_mappingHandle);
			_mappingHandle = nullptr;
		}
		if (_fileHandle != nullptr && _fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_fileHandle);
			_fileHandle = INVALID_HANDLE_VALUE;
		}
#elif defined(KEYH_PLATFORM_POSIX)
		if (_stringBuffer != nullptr)
		{
			munmap(_stringBuffer, _fileSize);
			_stringBuffer = nullptr;
		}
		if (_fileDescriptor != kInvalidFileDescriptor)
		{
			close(_fileDescriptor);
			_fileDescriptor = kInvalidFileDescriptor;
		}
#endif
		_fileSize = 0;
	}
}
