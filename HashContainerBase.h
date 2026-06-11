#pragma once
#include "HashUtil.h"

namespace keyh
{
	template <typename Derived>
	class HashContainerBase
	{
	protected:
		enum class InsertStatus
		{
			Inserted,
			Replaced,
			AlreadyExists,
			Error
		};

	protected:
		size_t _capacity = 0;
		size_t _capacityLevel = 0;
		size_t _size = 0;

	protected:
		template <typename Key, typename... Args>
		auto insertImpl(bool replace, Key&& key, Args&&... args);

	protected:
		void rehashIfNeeded();
		void rehash(size_t newCapacity);

	public:
		void reserve(size_t newCapacity);
		void clear();
	};
}
#include "HashContainerBase.hpp"

