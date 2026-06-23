#pragma once
#include "StaticString.h"
#include "HashMap.h"
#include "Vector.h"
#include "Ptr.h"

namespace keyh
{
	template<typename T, typename Hasher = FNV1aHash<StaticString<T>>>
	class FlyweightString
	{
	public:
		FlyweightString() = default;
		~FlyweightString() = default;

	public:
		FlyweightString(const T* str);
		FlyweightString(const StaticString<T>& str);

	private:
		static Vector<Ptr<StaticString<T>>>				_stringContainer;
		static HashMap<size_t, Ptr<StaticString<T>>>	_stringMap;

	private:
		size_t _index = 0;

	public:

	};
}
#include "FlyweightString.hpp"