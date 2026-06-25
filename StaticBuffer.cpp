#include "CommonBasePch.h"
#include "StaticBuffer.h"

namespace keyh
{
	template class StaticBuffer<char, kBuffer256Bytes>;
	template class StaticBuffer<char, kBuffer1KBytes>;
	template class StaticBuffer<char, kBuffer2KBytes>;
	template class StaticBuffer<char, kBuffer4KBytes>;
	template class StaticBuffer<wchar_t, kBuffer256Bytes>;
	template class StaticBuffer<wchar_t, kBuffer1KBytes>;
	template class StaticBuffer<wchar_t, kBuffer2KBytes>;
	template class StaticBuffer<wchar_t, kBuffer4KBytes>;

	using StaticBufferA256Bytes = StaticBuffer<char, kBuffer256Bytes>;
	using StaticBufferA1KBytes = StaticBuffer<char, kBuffer1KBytes>;
	using StaticBufferA2KBytes = StaticBuffer<char, kBuffer2KBytes>;
	using StaticBufferA4KBytes = StaticBuffer<char, kBuffer4KBytes>;

	using StaticBufferW256Bytes = StaticBuffer<wchar_t, kBuffer256Bytes>;
	using StaticBufferW1KBytes = StaticBuffer<wchar_t, kBuffer1KBytes>;
	using StaticBufferW2KBytes = StaticBuffer<wchar_t, kBuffer2KBytes>;
	using StaticBufferW4KBytes = StaticBuffer<wchar_t, kBuffer4KBytes>;

}