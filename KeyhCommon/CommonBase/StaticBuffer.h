#pragma once
#include "IBuffer.h"
#include "StrUtil.h"
namespace keyh
{
	template<typename T, size_t Size> class StaticBuffer;
	template<size_t Size> using StaticBufferA = StaticBuffer<char, Size>;
	template<size_t Size> using StaticBufferW = StaticBuffer<wchar_t, Size>;

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

	private:
		static void writeAscii(StaticBuffer* buffer, const char* value);
		template<typename ValueType>
		static void writeFormatValue(StaticBuffer* buffer, T specifier, ValueType value);
		template<typename ValueType>
		static void writeSignedFormatValue(StaticBuffer* buffer, T specifier, ValueType value);
		template<typename ValueType>
		static void writeUnsignedFormatValue(StaticBuffer* buffer, T specifier, ValueType value);
		template<typename ValueType>
		static void writeFloatFormatValue(StaticBuffer* buffer, T specifier, ValueType value);
		static void formatImpl(StaticBuffer* buffer, const T* formatString);
		template<typename ValueType, typename... Args>
		static void formatImpl(StaticBuffer* buffer, const T* formatString, ValueType value, Args... args);

	public:
		StaticBuffer() = default;
		~StaticBuffer() override = default;

		template<typename... Args>
		void format(const T* formatString, Args... args);

	protected:
		uint8 _buffer[Size] = { 0 };

	protected:
		inline void			resetImpl() { _buffer[0] = T(); }
		constexpr size_t	getCapacityImpl() const { return Size; }
		inline T*			getBufferImpl() { return reinterpret_cast<T*>(_buffer); }
		inline const T*		getBufferImpl() const { return reinterpret_cast<const T*>(_buffer); }
	};

}

#include "StaticBuffer.hpp"

