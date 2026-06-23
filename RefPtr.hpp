#include "RefPtr.h"
namespace keyh
{
	template<typename Base, typename Derived, typename ...Args>
	RefPtr<Base> makeRefPtr(Args ...args)
	{
		return RefPtr<Base>(new Derived(keyh::forward<Args>(args)...));
	}

	template<typename T>
	void RefPtr<T>::assignRefControlBlock()
	{
		_refControlBlock = new RefControlBlock();
		_refControlBlock->addRef();
		_refControlBlock->addWeakRef();
	}

	template<typename T>
	RefPtr<T>::RefPtr(T* ptr)
		: _ptr(ptr)
	{
		assignRefControlBlock();
	}

	template<typename T>
	template<typename U>
	RefPtr<T>::RefPtr(U* ptr) 
		: _ptr(ptr)
	{
		assignRefControlBlock();
	}

	template<typename T>
	RefPtr<T>::~RefPtr()
	{
		release();
	}

	template<typename T>
	RefPtr<T>::RefPtr(const RefPtr<T>& other)
	{
		if (other != nullptr)
		{
			other._refControlBlock->addRef();
			_refControlBlock = other._refControlBlock;
			_ptr = other._ptr;
		}
	}

	template<typename T>
	RefPtr<T>& RefPtr<T>::operator=(const RefPtr<T>&other)
	{
		if (*this != other)
		{
			release();

			if (other != nullptr)
			{
				other._refControlBlock->addRef();
				_refControlBlock = other._refControlBlock;
				_ptr = other._ptr;
			}
		}
		return *this;
	}
	
	template<typename T>
	RefPtr<T>& RefPtr<T>::operator=(Nullptr_t)
	{
		release();
		return *this;
	}

	template<typename T>
	template<typename U>
	RefPtr<T>::RefPtr(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (other != nullptr)
		{
			other._refControlBlock->addRef();
			_refControlBlock = other._refControlBlock;
			_ptr = other._ptr;
		}
	}

	template<typename T>
	template<typename U>
	RefPtr<T>& RefPtr<T>::operator=(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (*this != other)
		{
			release();

			if (other != nullptr)
			{
				other._refControlBlock->addRef();
				_refControlBlock = other._refControlBlock;
				_ptr = other._ptr;
			}
		}
		return *this;
	}

	template<typename T>
	void RefPtr<T>::release()
	{
		if (_refControlBlock != nullptr && _refControlBlock->release())
		{
			delete _ptr;
			if (_refControlBlock->releaseWeak())
			{
				delete _refControlBlock;
			}
			_ptr = nullptr;
			_refControlBlock = nullptr;
		}
	}
}