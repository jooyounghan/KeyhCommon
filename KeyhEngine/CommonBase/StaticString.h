#pragma once
#include "StrUtil.h"

namespace keyh
{
	template<typename T>
	class StaticString
	{
	private:
		static constexpr size_t kSsoMask = 1ULL << 63;
		static constexpr size_t kLengthMask = ~kSsoMask;
		static T gNullChar;

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
			T _ssoBuffer[StrUtil::ssoCapacity];
			T* _heap;
		};
		size_t _stringInfo;

	private:
		inline size_t getLength() const { return _stringInfo & kLengthMask; }
		inline void setLength(size_t length) { _stringInfo = (_stringInfo & kSsoMask) | (length & kLengthMask); }
		inline bool isSso() const { return (_stringInfo & kSsoMask) != 0; }
		inline void setSso(bool sso) { _stringInfo = (_stringInfo & kLengthMask) | (sso ? kSsoMask : 0); }

	public:
		inline const T* c_str() const { return isSso() ? _ssoBuffer : _heap; }
		inline size_t length() const { return getLength(); }
		inline size_t size() const { return getLength(); }
		inline bool empty() const { return getLength() == 0; }

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