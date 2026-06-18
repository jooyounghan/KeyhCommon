#pragma once
#include "CommonCore.h"
namespace keyh
{
	template<typename T>
	class Ptr;

	template<typename Base, typename Derived = Base, typename ...Args>
	Ptr<Base> makePtr(Args... args);

	template<typename T>
	class Ptr
	{
		template<typename U>
		friend class Ptr;

		template<typename Base, typename Derived, typename ...Args>
		friend Ptr<Base> makePtr(Args... args);

	private:
		Ptr(T* ptr = nullptr);
		template<typename U>
		Ptr(U* ptr);
		Ptr(const Ptr<T>& other) = delete;

	public:
		Ptr(Ptr<T>&& other) noexcept;
		~Ptr();

	public:
		Ptr<T>& operator=(const Ptr<T>& other) = delete;
		Ptr<T>& operator=(Ptr<T>&& other) noexcept;
		Ptr<T>& operator=(Nullptr_t);

	public:
		template<typename U>
		Ptr<T>& operator=(const Ptr<U>& other) = delete;
		template<typename U>
		Ptr<T>& operator=(Ptr<U>&& other);


	public:
		inline bool operator==(Nullptr_t) { return _ptr == nullptr; }
		inline bool operator!=(Nullptr_t) { return _ptr != nullptr; }

	public:
		inline T* get() const { return _ptr; }
		inline T* operator->() const { return _ptr; }

	private:
		void release();

	private:
		T* _ptr = nullptr;
	};
}
#include "Ptr.hpp"	
