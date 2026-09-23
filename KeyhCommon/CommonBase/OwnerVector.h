#pragma once
#include "Ptr.h"
#include "Vector.h"
#include "AssertUtil.h"

namespace keyh
{
	template<typename T>
	class OwnerVector
	{
	public:
		class Iterator
		{
		public:
			Iterator(Ptr<T>* ptr);

		public:
			inline T* operator*() { return _ptr->get(); }
			inline const T* operator*() const { return _ptr->get(); }
			inline T* operator->() { return _ptr->get(); }
			inline const T* operator->() const { return _ptr->get(); }
			inline bool operator==(const Iterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const Iterator& other) const { return !(*this == other); }

		public:
			Iterator& operator++();
			Iterator operator++(int);

		private:
			Ptr<T>* _ptr;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const Ptr<T>* ptr);

		public:
			inline const T* operator*() const { return _ptr->get(); }
			inline const T* operator->() const { return _ptr->get(); }
			inline bool operator==(const ConstIterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const ConstIterator& other) const { return !(*this == other); }

		public:
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const Ptr<T>* _ptr;
		};

	public:
		OwnerVector() = default;
		~OwnerVector() = default;

	public:
		OwnerVector(const OwnerVector&) = delete;
		OwnerVector& operator=(const OwnerVector&) = delete;

	public:
		OwnerVector(OwnerVector&&) noexcept = default;
		OwnerVector& operator=(OwnerVector&&) noexcept = default;

	public:
		T* push_back(Ptr<T>&& ptr);
		template<typename Derived = T, typename... Args>
		T* emplace_back(Args&&... args);
		void pop_back();

	public:
		inline Iterator begin() { return Iterator(_data.data()); }
		inline Iterator end() { return Iterator(_data.data() + _data.size()); }
		inline ConstIterator begin() const { return ConstIterator(_data.data()); }
		inline ConstIterator end() const { return ConstIterator(_data.data() + _data.size()); }

	public:
		T* operator[](size_t index);
		const T* operator[](size_t index) const;

	public:
		void erase(size_t index, bool keepOrder = true);

	public:
		void reserve(size_t newCapacity);
		void shrinkToFit();
		void clear();
		void swap(OwnerVector& other) noexcept;

	public:
		inline size_t size() const { return _data.size(); }
		inline size_t capacity() const { return _data.capacity(); }

	private:
		Vector<Ptr<T>> _data;
	};
}
#include "OwnerVector.hpp"
