#pragma once
#include "RefPtr.h"

namespace keyh
{
	template<typename T>
	class WeakPtr
	{
	public:
		WeakPtr() = default;
		WeakPtr(const WeakPtr<T>& other);
		WeakPtr<T>& operator=(const WeakPtr<T>& other);
		~WeakPtr();

	public:
		WeakPtr(const RefPtr<T>& other);
		WeakPtr<T>& operator=(const RefPtr<T>& other);
		WeakPtr<T>& operator=(Nullptr_t);

	public:
		template<typename U>
		WeakPtr(const RefPtr<U>& other);
		template<typename U>
		WeakPtr<T>& operator=(const RefPtr<U>& other);

	public:
		inline bool operator==(Nullptr_t) const { return _ptr == nullptr; }
		inline bool operator!=(Nullptr_t) const { return _ptr != nullptr; }
		inline bool operator==(const RefPtr<T>& other) const { return _ptr == other._ptr; }
		inline bool operator!=(const RefPtr<T>& other) const { return _ptr != other._ptr; }
		template<typename U>
		inline bool operator==(const RefPtr<U>& other) const { return _ptr == other._ptr; }
		template<typename U>
		inline bool operator!=(const RefPtr<U>& other) const { return _ptr != other._ptr; }
		inline bool operator==(const WeakPtr<T>& other) const { return _ptr == other._ptr; }
		inline bool operator!=(const WeakPtr<T>& other) const { return _ptr != other._ptr; }

	public:
		bool isValid() const;

	public:
		inline T* get() const { return _ptr; }
		inline T* operator->() const { return _ptr; }

	private:
		inline void assignWeakFrom(T* ptr, RefControlBlock* block);
		void releaseWeak();

	private:
		T*					_ptr = nullptr;
		RefControlBlock*	_refControlBlock = nullptr;
	};
}
#include "WeakPtr.hpp"
