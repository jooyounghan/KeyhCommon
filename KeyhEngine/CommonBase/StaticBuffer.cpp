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
}