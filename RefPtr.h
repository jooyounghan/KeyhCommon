#pragma once
#include "AssertUtil.h"
#include "RefControlBlock.h"

namespace keyh
{
	template<typename T>
	class RefPtr;
		
	template<typename Base, typename Derived = Base, typename ...Args>
	RefPtr<Base> makeRefPtr(Args... args);

	template<typename T>
	class RefPtr
	{
		template<typename U>
		friend class RefPtr;

		template<typename U>
		friend class WeakPtr;

	private:
		void assignRefControlBlock();

	private:
		RefPtr(T* ptr = nullptr);
		template<typename U>
		RefPtr(U* ptr);

	public:
		~RefPtr();

	public:
		RefPtr(const RefPtr<T>& other);
		RefPtr<T>& operator=(const RefPtr<T>& other);
		RefPtr<T>& operator=(Nullptr_t);

	public:
		template<typename U>
		RefPtr(const RefPtr<U>& other);
		template<typename U>
		RefPtr<T>& operator=(const RefPtr<U>& other);

	public:
		inline bool operator==(Nullptr_t) const { return _ptr == nullptr; }
		inline bool operator!=(Nullptr_t) const { return _ptr != nullptr; }
		inline bool operator==(const RefPtr<T>& other) const { return _ptr == other._ptr; }
		inline bool operator!=(const RefPtr<T>& other) const { return _ptr != other._ptr; }
		template<typename U>
		inline bool operator==(const RefPtr<U>& other) const { return _ptr == other._ptr; }
		template<typename U>
		inline bool operator!=(const RefPtr<U>& other) const { return _ptr != other._ptr; }

	public:
		inline T* get() const { return _ptr; }
		inline T* operator->() const { return _ptr; }

	public:
		void release();

	private:
		inline void assignFrom(T* ptr, RefControlBlock* block);

	private:
		T*					_ptr = nullptr;
		RefControlBlock*	_refControlBlock = nullptr;
	};

}
#include "RefPtr.hpp"
