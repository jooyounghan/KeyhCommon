#pragma once
namespace keyh
{
	using byte = unsigned char;
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;

	using int8 = char;
	using int16 = short;
	using int32 = int;
	using int64 = long long;

	struct TypeCommon
	{
		template<typename T>
		static constexpr void requireIntegral() noexcept;
	};

	template <typename T>
	struct RemoveReference
	{
		using type = T;
	};

	template <typename T>
	struct RemoveReference<T&>
	{
		using type = T;
	};

	template <typename T>
	struct RemoveReference<T&&>
	{
		using type = T;
	};

	template <typename T>
	constexpr typename RemoveReference<T>::type&& move(T&& arg) noexcept
	{
		return static_cast<typename RemoveReference<T>::type&&>(arg);
	}
}
#include "TypeCommon.hpp"