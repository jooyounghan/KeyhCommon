#pragma once
#include "CommonCore.h"

enum class TapeType : uint64_t
{
    ObjectStart = '{',
    ObjectEnd = '}',
    String = '"',
    Integer = 'l'
};

struct TapeElement
{
    uint64_t _value;

    void setElement(TapeType type, uint64_t payload)
    {
        uint64_t typeVal = static_cast<uint64_t>(type);
        _value = (typeVal << 56) | (payload & 0x00FFFFFFFFFFFFFFULL);
    }

    TapeType getType() const
    {
        return static_cast<TapeType>(_value >> 56);
    }

    uint64_t getPayload() const
    {
        return _value & 0x00FFFFFFFFFFFFFFULL;
    }
};

// Copy-on-Write(CoW) 기반의 크로스 플랫폼 파일 메모리 매핑 헬퍼 클래스
class FileMappingHelper
{
private:
    char* _stringBuffer;
    size_t _fileSize;

#if defined(_WIN32) || defined(_WIN64) || defined(_XBOX)
    HANDLE _fileHandle;
    HANDLE _mappingHandle;
#elif defined(__linux__) || defined(__APPLE__) || defined(__ORBIS__) || defined(__PROSPERO__)
    int _fileDescriptor;
#endif

public:
    FileMappingHelper()
    {
        _stringBuffer = nullptr;
        _fileSize = 0;

#if defined(_WIN32) || defined(_WIN64) || defined(_XBOX)
        _fileHandle = INVALID_HANDLE_VALUE;
        _mappingHandle = nullptr;
#elif defined(__linux__) || defined(__APPLE__) || defined(__ORBIS__) || defined(__PROSPERO__)
        _fileDescriptor = -1;
#endif
    }

    ~FileMappingHelper()
    {
        unload();
    }

    bool load(const char* filePath)
    {
#if defined(_WIN32) || defined(_WIN64) || defined(_XBOX)
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

        // PAGE_WRITECOPY: 디스크의 원본 파일 보호
        _mappingHandle = CreateFileMappingA(_fileHandle, nullptr, PAGE_WRITECOPY, 0, 0, nullptr);
        if (_mappingHandle == nullptr)
        {
            return false;
        }

        // FILE_MAP_COPY: 메모리 맵 뷰에서 변경이 일어나면 OS가 해당 페이지만 메모리 상에서 복제함
        _stringBuffer = static_cast<char*>(MapViewOfFile(_mappingHandle, FILE_MAP_COPY, 0, 0, 0));
        return _stringBuffer != nullptr;

#elif defined(__linux__) || defined(__APPLE__) || defined(__ORBIS__) || defined(__PROSPERO__)
        _fileDescriptor = open(filePath, O_RDONLY);
        if (_fileDescriptor == -1)
        {
            return false;
        }

        struct stat fileInfo;
        if (fstat(_fileDescriptor, &fileInfo) == -1)
        {
            return false;
        }
        _fileSize = static_cast<size_t>(fileInfo.st_size);

        // MAP_PRIVATE: 쓰기 작업 발생 시 원본 파일을 수정하지 않고 개별 복사본(CoW)을 생성함
        void* mappedMemory = mmap(nullptr, _fileSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, _fileDescriptor, 0);
        if (mappedMemory == MAP_FAILED)
        {
            return false;
        }

        _stringBuffer = static_cast<char*>(mappedMemory);
        return true;
#else
        return false; // 지원하지 않는 플랫폼
#endif
    }

    void unload()
    {
#if defined(_WIN32) || defined(_WIN64) || defined(_XBOX)
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
#elif defined(__linux__) || defined(__APPLE__) || defined(__ORBIS__) || defined(__PROSPERO__)
        if (_stringBuffer != nullptr)
        {
            munmap(_stringBuffer, _fileSize);
            _stringBuffer = nullptr;
        }
        if (_fileDescriptor != -1)
        {
            close(_fileDescriptor);
            _fileDescriptor = -1;
        }
#endif
        _fileSize = 0;
    }

    char* getStringBuffer() const
    {
        return _stringBuffer;
    }

    size_t getFileSize() const
    {
        return _fileSize;
    }
};