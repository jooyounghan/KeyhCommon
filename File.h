#pragma once
#include "CommonCore.h"

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
