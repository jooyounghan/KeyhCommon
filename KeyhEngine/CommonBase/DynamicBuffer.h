#pragma once
#include "IBuffer.h"
namespace keyh
{
	template<typename T>
	class DynamicBuffer : public IBuffer<T, DynamicBuffer<T>>
	{
		using Base = IBuffer<T, DynamicBuffer<T>>;
		using Base::_offset;
		friend class Base;

	public:
		DynamicBuffer() = default;
		~DynamicBuffer() override = default;

	protected:
		T* _buffer = nullptr;
		size_t _capacity = 0;

	private:
		void allocateInner(size_t size);

	public:
		void allocate(size_t size);
		void shrinkToFit();

	protected:
		void resetImpl();
		inline size_t getCapacityImpl() const { return _capacity; }
		inline T* getBufferImpl() { return _buffer; }
	};

	using DynamicBufferA = DynamicBuffer<char>;
	using DynamicBufferW = DynamicBuffer<wchar_t>;
}

