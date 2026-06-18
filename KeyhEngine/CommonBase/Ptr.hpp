namespace keyh
{
	template<typename Base, typename Derived, typename ...Args>
	Ptr<Base> makePtr(Args... args)
	{
		TypeTrait::requireDerivedFrom<Derived, Base>();
		return Ptr<Base>(new Derived(keyh::forward<Args>(args)...));
	}

	template<typename T>
	Ptr<T>::Ptr(T* ptr)
		: _ptr(ptr)
	{

	}

	template<typename T>
	template<typename U>
	Ptr<T>::Ptr(U* ptr)
		: _ptr(ptr)
	{
		TypeTrait::requireDerivedFrom<U, T>();
	}

	template<typename T>
	Ptr<T>::Ptr(Ptr<T>&& other) noexcept
	{
		_ptr = other._ptr;
		other._ptr = nullptr;
	}

	template<typename T>
	Ptr<T>::~Ptr()
	{
		release();
	}

	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Ptr<T>&& other) noexcept
	{
		if (_ptr != other._ptr)
		{
			release();
			_ptr = other._ptr;
			other._ptr = nullptr;
		}
		return *this;
	}

	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Nullptr_t)
	{
		release();
		return *this;
	}

	template<typename T>
	template<typename U>
	Ptr<T>& Ptr<T>::operator=(Ptr<U>&& other)
	{
		TypeTrait::requireDerivedFrom<U, T>();
		if (_ptr != other._ptr)
		{
			release();
			_ptr = other._ptr;
			other._ptr = nullptr;
		}
		return *this;
	}

	template<typename T>
	void Ptr<T>::release()
	{
		if (_ptr != nullptr)
		{
			delete _ptr;
		}
		_ptr = nullptr;
	}
}
