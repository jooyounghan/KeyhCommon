#pragma once
#include "MemoryUtil.h"
#include "AssertUtil.h"

namespace keyh
{
	template<typename T, size_t Count>
	class InlinedVector
	{
		KEYH_STATIC_ASSERT(Count > 0, "InlinedVector inline capacity must be greater than zero.");

	public:
		class Iterator
		{
		public:
			Iterator(T* ptr);

		public:
			inline T& operator*() { return *_ptr; }
			inline const T& operator*() const { return *_ptr; }
			inline T* operator->() { return _ptr; }
			inline const T* operator->() const { return _ptr; }
			inline operator T* () { return _ptr; }
			inline operator const T* () const { return _ptr; }
			inline bool operator==(const Iterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const Iterator& other) const { return !(*this == other); }
			inline Iterator operator+(ptrdiff_t offset) const { return Iterator(_ptr + offset); }
			inline Iterator operator-(ptrdiff_t offset) const { return Iterator(_ptr - offset); }
			inline ptrdiff_t operator-(const Iterator& other) const { return _ptr - other._ptr; }

		public:
			Iterator& operator++();
			Iterator operator++(int);

		private:
			T* _ptr;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const T* ptr);

		public:
			inline const T& operator*() const { return *_ptr; }
			inline const T* operator->() const { return _ptr; }
			inline operator const T* () const { return _ptr; }
			inline bool operator==(const ConstIterator& other) const { return _ptr == other._ptr; }
			inline bool operator!=(const ConstIterator& other) const { return !(*this == other); }
			inline ConstIterator operator+(ptrdiff_t offset) const { return ConstIterator(_ptr + offset); }
			inline ConstIterator operator-(ptrdiff_t offset) const { return ConstIterator(_ptr - offset); }
			inline ptrdiff_t operator-(const ConstIterator& other) const { return _ptr - other._ptr; }

		public:
			ConstIterator& operator++();
			ConstIterator operator++(int);

		private:
			const T* _ptr;
		};

	public:
		InlinedVector();
		~InlinedVector();

	public:
		InlinedVector(const InlinedVector& other);
		InlinedVector& operator=(const InlinedVector& other);

	public:
		InlinedVector(InlinedVector&& other) noexcept;
		InlinedVector& operator=(InlinedVector&& other) noexcept;

	private:
		static constexpr size_t kHeapMask = size_t(1) << (sizeof(size_t) * 8 - 1);
		static constexpr size_t kCapacityMask = ~kHeapMask;

		union Storage
		{
			Storage() : _heap(nullptr) {}
			~Storage() {}

			T _ssoBuffer[Count];
			T* _heap;
		} _storage;

		size_t _size = 0;
		size_t _capacityInfo = 0;

	private:
		inline size_t getHeapCapacity() const { return _capacityInfo & kCapacityMask; }
		inline void setHeapCapacity(size_t capacity) { _capacityInfo = (_capacityInfo & kHeapMask) | (capacity & kCapacityMask); }
		inline bool isHeapAllocated() const { return (_capacityInfo & kHeapMask) != 0; }
		inline void setHeapAllocated(bool heapAllocated) { _capacityInfo = (_capacityInfo & kCapacityMask) | (heapAllocated ? kHeapMask : 0); }
		inline T* getBuffer() { return isHeapAllocated() ? _storage._heap : _storage._ssoBuffer; }
		inline const T* getBuffer() const { return isHeapAllocated() ? _storage._heap : _storage._ssoBuffer; }
		void destroyRange(size_t beginIndex, size_t endIndex);

	public:
		T& push_back(const T& value);
		T& push_back(T&& value);
		template<typename... Args>
		T& emplace_back(Args&&... args);
		void pop_back();

	public:
		inline Iterator begin() { return Iterator(data()); }
		inline Iterator end() { return Iterator(data() + _size); }
		inline ConstIterator begin() const { return ConstIterator(data()); }
		inline ConstIterator end() const { return ConstIterator(data() + _size); }

	public:
		T& operator[](size_t index);
		const T& operator[](size_t index) const;

	public:
		void erase(size_t index, bool keepOrder = true);

	public:
		void resize(size_t newSize);
		void reserve(size_t newCapacity);
		void shrinkToFit();
		void clear();
		void swap(InlinedVector& other);

	public:
		inline const T* data() const { return getBuffer(); }
		inline size_t size() const { return _size; }
		inline size_t capacity() const { return isHeapAllocated() ? getHeapCapacity() : Count; }
		inline bool empty() const { return _size == 0; }
	};
}
#include "InlinedVector.hpp"
