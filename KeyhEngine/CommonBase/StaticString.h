#pragma once
#include "StrUtil.h"
namespace keyh
{
	template<typename T>
	class StaticString
	{
	public:
		StaticString() = default;
		~StaticString();

	public:
		StaticString(const T* str);

	public:
		StaticString(const StaticString& other);
		StaticString& operator=(const StaticString& other);

	public:
		StaticString(StaticString&& other) noexcept;
		StaticString& operator=(StaticString&& other) noexcept;
	
	private:
		struct SsoStorage
		{
			T _buffer[StrUtil::ssoCapacity];
		};

		struct HeapStorage
		{
			T* _data;
		};

		struct StringInfo
		{
			size_t _size : 63;
			size_t _isSso : 1;
		};

		union
		{
			SsoStorage _sso;
			HeapStorage _heap;
		};
		StringInfo _stringInfo;

	public:
		inline const T* c_str() const { return _data; }
		inline size_t size() const { return _size; }
		inline bool empty() const { return _size == 0; }

	public:
		bool operator==(const StaticString& other) const;
		bool operator!=(const StaticString& other) const;
		T& operator[](size_t index);
		const T& operator[](size_t index) const;

	public:
		inline T* begin() { return c_str(); }
		inline const T* begin() const { return c_str(); }
		inline T* end() { return c_str() + size(); }
		inline const T* end() const { return c_str() + size(); }
		inline const T* cbegin() const { return c_str(); }
		inline const T* cend() const { return c_str() + size(); }

	public:
		void clear();
	};
}
