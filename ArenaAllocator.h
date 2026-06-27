#pragma once
#include "CommonCore.h"
#include <type_traits>
namespace keyh
{
    typedef void (*DestructorFn)(void*);

    struct DeleterNode
    {
        DeleterNode* _next;
        DestructorFn    _destructor;
        void* _object;
    };

    struct MemoryChunk
    {
        char* _buffer;
        size_t          _capacity;
        size_t          _offset;
        MemoryChunk* _next;
    };

    class ArenaAllocator
    {
    public:
        ArenaAllocator(size_t defaultSize = 65536);
        ~ArenaAllocator();

    private:
        MemoryChunk* _headChunk;
        MemoryChunk* _currentChunk;
        DeleterNode* _headDeleter;
        size_t          _defaultChunkSize;

    private:
        void allocateNewChunk(size_t requiredSize);
        void registerDeleter(void* object, DestructorFn destructor);

    private:
        template <typename T>
        static void invokeDestructor(void* object);

    public:
        void* allocateMemory(size_t size);
        void clear();

    public:
        template <typename T, typename... Args>
        T* createObject(Args&&... args);
    };
}
#include "ArenaAllocator.hpp"

