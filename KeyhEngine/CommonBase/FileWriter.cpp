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

    bool FileWriter::writeRaw(const void* data, size_t size)
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

    bool FileWriter::flush()
    {
        if (_hasError)
            return false;

        const size_t pending = _writeBuffer.getSizeBytes();
        if (pending == 0)
            return true;

        const bool ok = writeRaw(_writeBuffer.getRawBuffer(), pending);
        _writeBuffer.reset();
        if (!ok)
            _hasError = true;
        return ok;
    }

    void FileWriter::close()
    {
        // Any flush failure is stored in _hasError and can be checked via hasError()
        // after an explicit close() call. The destructor cannot propagate errors.
        flush();

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
        writer.writeBytes(data, size);
        return writer.flush();
    }

    // -----------------------------------------------------------------------
    // IBuffer interface
    // -----------------------------------------------------------------------

    void FileWriter::writeBytes(const void* input, size_t size)
    {
        if (size == 0)
            return;

        // If the incoming chunk is larger than or equal to the full buffer
        // capacity, flush the pending buffer and write directly to the file.
        if (size >= getCapacityBytes())
        {
            flush();
            if (!writeRaw(input, size))
                _hasError = true;
            return;
        }

        // If the incoming chunk does not fit in the remaining space, flush first.
        if (size > _writeBuffer.getAvailableSizeBytes())
            flush();

        _writeBuffer.writeBytes(input, size);
    }

    void FileWriter::resetRaw()
    {
        _writeBuffer.reset();
    }

    size_t FileWriter::getSizeBytes() const
    {
        return _writeBuffer.getSizeBytes();
    }

    size_t FileWriter::getCapacityBytes() const
    {
        return kBuffer4KBytes;
    }

    void* FileWriter::getRawBuffer()
    {
        // FileWriter is a write-only streaming abstraction; direct buffer access is not supported.
        return nullptr;
    }

    const void* FileWriter::getRawBuffer() const
    {
        // FileWriter is a write-only streaming abstraction; direct buffer access is not supported.
        return nullptr;
    }

    size_t FileWriter::getAvailableSizeBytes() const
    {
        return _writeBuffer.getAvailableSizeBytes();
    }
}

