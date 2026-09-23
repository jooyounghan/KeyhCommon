#pragma once
#include <cstddef>

namespace keyh
{
	using utf8 = char;
	using byte = unsigned char;
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using int8 = char;
	using int16 = short;
	using int32 = int;
	using int64 = long long;

	constexpr uint32 kInvalidUint32 = static_cast<uint32>(-1);
	constexpr uint64 kInvalidUint64 = static_cast<uint64>(-1);
	constexpr size_t kInvalidSizeT = static_cast<size_t>(-1);
}