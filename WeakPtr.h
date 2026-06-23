#pragma once
#include "RefPtr.h"

namespace keyh
{
	template<typename T>
	class WeakPtr
	{
	public:
		WeakPtr() = default;
		WeakPtr(const RefPtr<T>& other);
		WeakPtr<T>& operator=(const RefPtr<T>& other);
		WeakPtr<T>& operator=(Nullptr_t);

	public:
		template<typename U>
		WeakPtr(const RefPtr<U>& other);
		template<typename U>
		WeakPtr<T>& operator=(const RefPtr<U>& other);

	public:
		bool isValid();

	public:
		inline T* get() const { return _ptr; }
		inline T* operator->() const { return _ptr; }

	private:
		T*					_ptr = nullptr;
		RefControlBlock*	_refControlBlock = nullptr;
	};
}
#include "WeakPtr.hpp"
