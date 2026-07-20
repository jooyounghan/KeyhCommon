#include "CommonBasePch.h"
#include "FileWriter.h"

#if defined(KEYH_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(KEYH_PLATFORM_POSIX)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace keyh
{
    FileWriter::~FileWriter() { close(); }

    bool FileWriter::open(const char* filePath)
    {
#if defined(KEYH_PLATFORM_WINDOWS)
        _fileHandle = CreateFileA(filePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        return _fileHandle != INVALID_HANDLE_VALUE;

#elif defined(KEYH_PLATFORM_POSIX)
        _fileDescriptor = ::open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        return _fileDescriptor != kInvalidFileDescriptor;

#else
        return false;
#endif
    }

    bool FileWriter::write(const void* data, size_t size)
    {
        if (size == 0)
            return true;

#if defined(KEYH_PLATFORM_WINDOWS)
        if (_fileHandle == nullptr || _fileHandle == INVALID_HANDLE_VALUE)
            return false;

        const char* cursor = static_cast<const char*>(data);
        size_t remaining = size;
        while (remaining > 0)
        {
            const DWORD toWrite = static_cast<DWORD>(remaining > 0xFFFFFFFFu ? 0xFFFFFFFFu : remaining);
            DWORD written = 0;
            if (!WriteFile(_fileHandle, cursor, toWrite, &written, nullptr))
                return false;
            cursor += written;
            remaining -= written;
        }
        return true;

#elif defined(KEYH_PLATFORM_POSIX)
        if (_fileDescriptor == kInvalidFileDescriptor)
            return false;

        const char* cursor = static_cast<const char*>(data);
        size_t remaining = size;
        while (remaining > 0)
        {
            const ssize_t written = ::write(_fileDescriptor, cursor, remaining);
            if (written < 0)
                return false;
            cursor += written;
            remaining -= static_cast<size_t>(written);
        }
        return true;

#else
        return false;
#endif
    }

    void FileWriter::close()
    {
#if defined(KEYH_PLATFORM_WINDOWS)
        if (_fileHandle != nullptr && _fileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(_fileHandle);
            _fileHandle = nullptr;
        }
#elif defined(KEYH_PLATFORM_POSIX)
        if (_fileDescriptor != kInvalidFileDescriptor)
        {
            ::close(_fileDescriptor);
            _fileDescriptor = kInvalidFileDescriptor;
        }
#endif
    }

    bool FileWriter::save(const char* filePath, const void* data, size_t size)
    {
        FileWriter writer;
        if (!writer.open(filePath))
            return false;
        return writer.write(data, size);
    }
}
