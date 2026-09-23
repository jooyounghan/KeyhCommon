namespace keyh
{
	template<typename T>
	OwnerVector<T>::Iterator::Iterator(Ptr<T>* ptr)
		: _ptr(ptr) {}

	template<typename T>
	typename OwnerVector<T>::Iterator& OwnerVector<T>::Iterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T>
	typename OwnerVector<T>::Iterator OwnerVector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++_ptr;
		return temp;
	}

	template<typename T>
	OwnerVector<T>::ConstIterator::ConstIterator(const Ptr<T>* ptr)
		: _ptr(ptr) {}

	template<typename T>
	typename OwnerVector<T>::ConstIterator& OwnerVector<T>::ConstIterator::operator++()
	{
		++_ptr;
		return *this;
	}

	template<typename T>
	typename OwnerVector<T>::ConstIterator OwnerVector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++_ptr;
		return temp;
	}

	template<typename T>
	T* OwnerVector<T>::push_back(Ptr<T>&& ptr)
	{
		_data.push_back(keyh::move(ptr));
		return _data[_data.size() - 1].get();
	}

	template<typename T>
	template<typename Derived, typename... Args>
	T* OwnerVector<T>::emplace_back(Args&&... args)
	{
		_data.push_back(makePtr<T, Derived>(keyh::forward<Args>(args)...));
		return _data[_data.size() - 1].get();
	}

	template<typename T>
	void OwnerVector<T>::pop_back()
	{
		_data.pop_back();
	}

	template<typename T>
	T* OwnerVector<T>::operator[](size_t index)
	{
		KEYH_ASSERT(index < _data.size(), "Index out of bounds");
		return _data[index].get();
	}

	template<typename T>
	const T* OwnerVector<T>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _data.size(), "Index out of bounds");
		return _data[index].get();
	}

	template<typename T>
	void OwnerVector<T>::erase(size_t index, bool keepOrder)
	{
		_data.erase(index, keepOrder);
	}

	template<typename T>
	void OwnerVector<T>::reserve(size_t newCapacity)
	{
		_data.reserve(newCapacity);
	}

	template<typename T>
	void OwnerVector<T>::shrinkToFit()
	{
		_data.shrinkToFit();
	}

	template<typename T>
	void OwnerVector<T>::clear()
	{
		_data.clear();
	}

	template<typename T>
	void OwnerVector<T>::swap(OwnerVector& other) noexcept
	{
		_data.swap(other._data);
	}
}
