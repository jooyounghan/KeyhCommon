#pragma once
#include "IBuffer.h"
namespace keyh
{
	constexpr size_t kBuffer256Bytes = 256;
	constexpr size_t kBuffer1KBytes = 1024;
	constexpr size_t kBuffer2KBytes = 2048;
	constexpr size_t kBuffer4KBytes = 4096;

	template<typename T, size_t Size>
	class StaticBuffer : public IBuffer<T, StaticBuffer<T, Size>>
	{
		using Base = IBuffer<T, StaticBuffer<T, Size>>;
		using Base::_offset;
		friend class Base;

	public:
		StaticBuffer() = default;
		~StaticBuffer() override = default;

	protected:
		uint8_t _buffer[Size] = { 0 };

	protected:
		inline void resetImpl() { _buffer[0] = T(); }
		constexpr size_t getCapacityImpl() const { return Size; }
		inline T* getBufferImpl() { return reinterpret_cast<T*>(_buffer); }
	};

	using StaticBufferA256Bytes = StaticBuffer<char, kBuffer256Bytes>;
	using StaticBufferA1KBytes = StaticBuffer<char, kBuffer1KBytes>;
	using StaticBufferA2KBytes = StaticBuffer<char, kBuffer2KBytes>;
	using StaticBufferA4KBytes = StaticBuffer<char, kBuffer4KBytes>;

	using StaticBufferW256Bytes = StaticBuffer<wchar_t, kBuffer256Bytes>;
	using StaticBufferW1KBytes = StaticBuffer<wchar_t, kBuffer1KBytes>;
	using StaticBufferW2KBytes = StaticBuffer<wchar_t, kBuffer2KBytes>;
	using StaticBufferW4KBytes = StaticBuffer<wchar_t, kBuffer4KBytes>;
}

