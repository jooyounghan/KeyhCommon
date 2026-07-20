#pragma once
#include "CommonCore.h"

namespace keyh
{
    class FileWriter
    {
    public:
        FileWriter() = default;
        ~FileWriter();

    private:
#if defined(KEYH_PLATFORM_WINDOWS)
        void* _fileHandle = nullptr;
#elif defined(KEYH_PLATFORM_POSIX)
        static constexpr int32 kInvalidFileDescriptor = -1;
        int32 _fileDescriptor = kInvalidFileDescriptor;
#endif

    public:
        bool open(const char* filePath);
        bool write(const void* data, size_t size);
        void close();

    public:
        static bool save(const char* filePath, const void* data, size_t size);
    };
}
