#pragma once
#include "IBuffer.h"
namespace keyh
{
	constexpr size_t kMaxPathLength = 260;
	constexpr size_t kBuffer128Bytes = 128;
	constexpr size_t kBuffer256Bytes = 256;
	constexpr size_t kBuffer1KBytes = 1024;
	constexpr size_t kBuffer2KBytes = 2048;
	constexpr size_t kBuffer4KBytes = 4096;

	template<typename T, size_t Size>
	class StaticBuffer : public IBufferBase<T, StaticBuffer<T, Size>>
	{
		using Base = IBufferBase<T, StaticBuffer<T, Size>>;
		using Base::_offset;
		friend class Base;

	public:
		StaticBuffer() = default;
		~StaticBuffer() override = default;

	protected:
		uint8 _buffer[Size] = { 0 };

	protected:
		inline void			resetImpl() { _buffer[0] = T(); }
		constexpr size_t	getCapacityImpl() const { return Size; }
		inline T*			getBufferImpl() { return reinterpret_cast<T*>(_buffer); }
		inline const T*		getBufferImpl() const { return reinterpret_cast<const T*>(_buffer); }
	};

	template<size_t Size>
	using StaticBufferA = StaticBuffer<char, Size>;

	template<size_t Size>
	using StaticBufferW = StaticBuffer<wchar_t, Size>;
}

