#pragma once
#include "StaticString.h"
#include "Vector.h"
#include "Ptr.h"

namespace keyh
{
	template<typename T>
	class FlyweightString
	{
	public:
		FlyweightString() = default;
		~FlyweightString() = default;

	public:
		FlyweightString(const T* str);
		FlyweightString(const StaticString<T>& str);

	private:

	private:
		size_t _index = 0;

	public:

	};
}