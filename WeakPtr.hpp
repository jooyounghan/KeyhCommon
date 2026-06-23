#include "WeakPtr.h"
namespace keyh
{
	template<typename T>
	WeakPtr<T>::WeakPtr(const RefPtr<T>& other)
	{
		if (other != nullptr)
		{
			other._refControlBlock->addWeakRef();
			_refControlBlock = other._refControlBlock;
			_ptr = other._ptr;
		}
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(const RefPtr<T>& other)
	{
		if (*this != other)
		{
			if (_refControlBlock != nullptr && _refControlBlock->releaseWeak())
			{
				delete _refControlBlock;
			}

			if (other != nullptr)
			{
				other._refControlBlock->addWeakRef();
				_refControlBlock = other._refControlBlock;
				_ptr = other._ptr;
			}
		}
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(Nullptr_t)
	{
		if (_refControlBlock != nullptr && _refControlBlock->releaseWeak())
		{
			delete _refControlBlock;
		}
		_refControlBlock = nullptr;
		_ptr = nullptr;
	}

	template<typename T>
	template<typename U>
	WeakPtr<T>::WeakPtr(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (other != nullptr)
		{
			other._refControlBlock->addWeakRef();
			_refControlBlock = other._refControlBlock;
			_ptr = other._ptr;
		}
	}

	template<typename T>
	template<typename U>
	WeakPtr<T>& WeakPtr<T>::operator=(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (*this != other)
		{
			if (_refControlBlock != nullptr && _refControlBlock->releaseWeak())
			{
				delete _refControlBlock;
			}
			if (other != nullptr)
			{
				other._refControlBlock->addWeakRef();
				_refControlBlock = other._refControlBlock;
				_ptr = other._ptr;
			}
		}
	}

	template<typename T>
	bool WeakPtr<T>::isValid()
	{
		if (_refControlBlock == nullptr)
			return false;

		return _refControlBlock->getRefCount() > 0;
	}
}