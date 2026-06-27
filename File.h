#pragma once
#include "CommonCore.h"

#if defined(KEYH_PLATFORM_WINDOWS)
#pragma region Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma endregion

#elif defined(KEYH_PLATFORM_POSIX)

#pragma region POSIX
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#pragma endregion
#endif

namespace keyh
{
    class File
    {
		static constexpr int32 kInvalidFileDescriptor = -1;

    public:
        File() = default;
        ~File();

    private:
        char*   _stringBuffer = nullptr;
        size_t  _fileSize = 0;

#if defined(KEYH_PLATFORM_WINDOWS)
        HANDLE _fileHandle = INVALID_HANDLE_VALUE;
        HANDLE _mappingHandle = nullptr;
#elif defined(KEYH_PLATFORM_POSIX)
        int32 _fileDescriptor = kInvalidFileDescriptor;
#endif

    public:
		inline const char* getStringBuffer() const { return _stringBuffer; }
        inline size_t getFileSize() const { return _fileSize; }

    public:
		bool load(const char* filePath);
        void unload();
    };
}
