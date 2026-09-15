#pragma once
#include "StrUtil.h"

namespace keyh
{
	template<typename T>
	class StaticString
	{
	private:
		static constexpr size_t kHeapMask = size_t(1) << (sizeof(size_t) * 8 - 1);
		static constexpr size_t kCapacityMask = ~kHeapMask;
		static T gNullChar;

	public:
		static const StaticString Empty;

	public:
		StaticString();
		~StaticString();

	public:
		StaticString(const T* str);
		StaticString(const T* str, size_t length);

	public:
		StaticString(const StaticString& other);
		StaticString& operator=(const StaticString& other);

	public:
		StaticString(StaticString&& other) noexcept;
		StaticString& operator=(StaticString&& other) noexcept;

	private:
		union
		{
			T _ssoBuffer[StrUtil::kSsoCapacity];
			T* _heap;
		};
		size_t _size;
		size_t _capacityInfo;

	private:
		inline size_t getHeapCapacity() const { return _capacityInfo & kCapacityMask; }
		inline void setHeapCapacity(size_t capacity) { _capacityInfo = (_capacityInfo & kHeapMask) | (capacity & kCapacityMask); }
		inline bool isHeapAllocated() const { return (_capacityInfo & kHeapMask) != 0; }
		inline void setHeapAllocated(bool heapAllocated) { _capacityInfo = (_capacityInfo & kCapacityMask) | (heapAllocated ? kHeapMask : 0); }

	public:
		inline const T* c_str() const { return isHeapAllocated() ? _heap : _ssoBuffer; }
		inline size_t length() const { return _size; }
		inline size_t size() const { return _size * sizeof(T); }
		inline size_t capacity() const { return isHeapAllocated() ? getHeapCapacity() : StrUtil::kSsoCapacity; }
		inline bool empty() const { return _size == 0; }

	public:
		bool operator==(const StaticString& other) const;
		bool operator!=(const StaticString& other) const;
		T& operator[](size_t index);
		const T& operator[](size_t index) const;

	public:
		void swap(StaticString& other) noexcept;
		void clear();
	};

	using StaticStringA = StaticString<char>;
	using StaticStringW = StaticString<wchar_t>;
}

#pragma region FNV1aHash Specializations
#include "FNV1aHasher.h"
namespace keyh
{
	DECLARE_TEMPLATED_FNV1A_HASH_SPECIALIZATION(StaticString, char);
	DECLARE_TEMPLATED_FNV1A_HASH_SPECIALIZATION(StaticString, wchar_t);
}
#pragma endregion