#pragma once
#include "IBuffer.h"
namespace keyh
{
	template<typename T, bool isString = false>
	class DynamicBuffer : public IBufferBase<T, DynamicBuffer<T, isString>>
	{
		using Base = IBufferBase<T, DynamicBuffer<T, isString>>;
		using Base::_offset;
		friend class Base;

	public:
		static constexpr bool kIsString = isString;
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
		inline size_t	getCapacityImpl() const { return _capacity; }
		inline size_t	getWritableCapacityImpl() const { return (_capacity - (_capacity != 0 && isString ? 1 : 0)) * sizeof(T); }
		inline T*		getBufferImpl() { return _buffer; }
		inline const T* getBufferImpl() const { return _buffer; }
	};

	template<typename T> using DynamicDataBuffer = DynamicBuffer<T, false>;
	template<typename T> using DynamicStringBuffer = DynamicBuffer<T, true>;
	using DynamicBufferA = DynamicStringBuffer<char>;
	using DynamicBufferW = DynamicStringBuffer<wchar_t>;
}

