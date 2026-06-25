#pragma once
#include "IBuffer.h"
namespace keyh
{
	template<typename T>
	class DynamicBuffer : public IBuffer<T, DynamicBuffer<T>>
	{
		using Base = IBuffer<T, DynamicBuffer<T>>;
		using Base::_offset;

	public:
		DynamicBuffer() = default;
		~DynamicBuffer() override = default;

	public:
		T* _buffer = nullptr;
		size_t _capacity = 0;

	public:
		void allocate(size_t size);

	protected:
		void resetImpl();
		inline size_t getCapacityImpl() const { return _capacity; }
	};
}

