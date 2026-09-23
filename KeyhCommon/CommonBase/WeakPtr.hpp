namespace keyh
{
	template<typename T>
	inline void WeakPtr<T>::assignWeakFrom(T* ptr, RefControlBlock* block)
	{
		if (block != nullptr)
		{
			block->addWeakRef();
			_ptr = ptr;
			_refControlBlock = block;
		}
	}

	template<typename T>
	void WeakPtr<T>::releaseWeak()
	{
		if (_refControlBlock != nullptr)
		{
			if (_refControlBlock->releaseWeak())
			{
				delete _refControlBlock;
			}
			_ptr = nullptr;
			_refControlBlock = nullptr;
		}
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
	{
		assignWeakFrom(other._ptr, other._refControlBlock);
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
	{
		if (*this != other)
		{
			releaseWeak();
			assignWeakFrom(other._ptr, other._refControlBlock);
		}
		return *this;
	}

	template<typename T>
	WeakPtr<T>::~WeakPtr()
	{
		releaseWeak();
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(const RefPtr<T>& other)
	{
		assignWeakFrom(other._ptr, other._refControlBlock);
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(const RefPtr<T>& other)
	{
		if (*this != other)
		{
			releaseWeak();
			assignWeakFrom(other._ptr, other._refControlBlock);
		}
		return *this;
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(Nullptr_t)
	{
		releaseWeak();
		return *this;
	}

	template<typename T>
	template<typename U>
	WeakPtr<T>::WeakPtr(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		assignWeakFrom(other._ptr, other._refControlBlock);
	}

	template<typename T>
	template<typename U>
	WeakPtr<T>& WeakPtr<T>::operator=(const RefPtr<U>& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (*this != other)
		{
			releaseWeak();
			assignWeakFrom(other._ptr, other._refControlBlock);
		}
		return *this;
	}

	template<typename T>
	bool WeakPtr<T>::isValid() const
	{
		if (_refControlBlock == nullptr)
			return false;

		return _refControlBlock->getRefCount() > 0;
	}
}