#include "CommonBasePch.h"
#include "ArenaAllocator.h"

namespace keyh
{
	ArenaAllocator::ArenaAllocator(size_t defaultSize)
	{
		_headChunk = nullptr;
		_currentChunk = nullptr;
		_headDeleter = nullptr;
		_defaultChunkSize = defaultSize;
	}
	
	ArenaAllocator::~ArenaAllocator()
	{
		clear();
	}


	void ArenaAllocator::clear()
	{
		DeleterNode* currentDeleter = _headDeleter;
		while (currentDeleter != nullptr)
		{
			currentDeleter->_destructor(currentDeleter->_object);
			currentDeleter = currentDeleter->_next;
		}
		_headDeleter = nullptr;

		MemoryChunk* currentChunk = _headChunk;
		while (currentChunk != nullptr)
		{
			MemoryChunk* next = currentChunk->_next;
			free(currentChunk);
			currentChunk = next;
		}
		_headChunk = nullptr;
		_currentChunk = nullptr;
	}

	void* ArenaAllocator::allocateMemory(size_t size)
	{
		size_t alignedSize = (size + 7) & ~7;

		if (_currentChunk == nullptr || _currentChunk->_offset + alignedSize > _currentChunk->_capacity)
		{
			allocateNewChunk(alignedSize);
		}

		void* result = _currentChunk->_buffer + _currentChunk->_offset;
		_currentChunk->_offset += alignedSize;

		return result;
	}

	void ArenaAllocator::allocateNewChunk(size_t requiredSize)
	{
		size_t chunkSize = requiredSize > _defaultChunkSize ? requiredSize : _defaultChunkSize;
		char* memory = static_cast<char*>(malloc(sizeof(MemoryChunk) + chunkSize));
		MemoryChunk* newChunk = reinterpret_cast<MemoryChunk*>(memory);
		newChunk->_buffer = memory + sizeof(MemoryChunk);
		newChunk->_capacity = chunkSize;
		newChunk->_offset = 0;
		newChunk->_next = nullptr;
		if (_headChunk == nullptr)
		{
			_headChunk = newChunk;
			_currentChunk = newChunk;
		}
		else
		{
			_currentChunk->_next = newChunk;
			_currentChunk = newChunk;
		}
	}

	void ArenaAllocator::registerDeleter(void* object, DestructorFn destructor)
	{
		void* deleterMemory = allocateMemory(sizeof(DeleterNode));
		DeleterNode* node = new (deleterMemory) DeleterNode();
		node->_object = object;
		node->_destructor = destructor;
		node->_next = _headDeleter;
		_headDeleter = node;
	}

}