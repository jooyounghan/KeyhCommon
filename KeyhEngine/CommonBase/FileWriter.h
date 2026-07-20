#pragma once
#include "CommonCore.h"
#include "IBuffer.h"
#include "StaticBuffer.h"

namespace keyh
{
    class FileWriter : public IBuffer
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

    private:
        StaticBufferA4KBytes _writeBuffer;
        bool _hasError = false;

    public:
        bool open(const char* filePath);
        bool flush();
        void close();
        bool hasError() const { return _hasError; }

    private:
        bool writeRaw(const void* data, size_t size);

    public:
        static bool save(const char* filePath, const void* data, size_t size);

    public:
        virtual void        writeBytes(const void* input, size_t size) override;
        virtual void        resetRaw() override;
        virtual size_t      getSizeBytes() const override;
        virtual size_t      getCapacityBytes() const override;
        virtual void*       getRawBuffer() override;
        virtual const void* getRawBuffer() const override;
        virtual size_t      getAvailableSizeBytes() const override;
    };
}
