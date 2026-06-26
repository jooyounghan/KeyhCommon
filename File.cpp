#include "CommonBasePch.h"
#include "File.h"

namespace keyh
{
	File::~File() { unload(); }

	bool File::load(const char* filePath)
	{
#if defined(KEYH_PLATFORM_WINDOWS)
		_fileHandle = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (_fileHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		LARGE_INTEGER size;
		if (!GetFileSizeEx(_fileHandle, &size))
		{
			return false;
		}
		_fileSize = static_cast<size_t>(size.QuadPart);

		const DWORD sizeHigh = static_cast<DWORD>(_fileSize >> 32);
		const DWORD sizeLow = static_cast<DWORD>(_fileSize & 0xFFFFFFFF);

		_mappingHandle = CreateFileMappingA(_fileHandle, nullptr, PAGE_WRITECOPY, sizeHigh, sizeLow, nullptr);
		if (_mappingHandle == nullptr)
		{
			return false;
		}

		_stringBuffer = static_cast<char*>(MapViewOfFile(_mappingHandle, FILE_MAP_COPY, 0, 0, _fileSize));
		return _stringBuffer != nullptr;

#elif defined(KEYH_PLATFORM_POSIX)
		_fileDescriptor = open(filePath, O_RDONLY);
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
		if (_fileHandle != INVALID_HANDLE_VALUE)
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