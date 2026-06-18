#pragma once
#include "StrUtil.h"

namespace keyh
{
	template<typename T>
	class StringView
	{
	public:
		StringView() = default;
		~StringView() = default;

	public:
		inline StringView(const T* data, size_t length) : _data(data), _length(length) {}
		inline StringView(const T* data) : _data(data), _length(StrUtil::length(data)) {}
		
	private:
		const T* _data = nullptr;
		size_t _length = 0;

	public:
		inline const T* data() const { return _data; }
		inline size_t length() const { return _length; }
		inline size_t size() const { return _length; }
		inline bool empty() const { return _length == 0; }

	public:
		bool operator==(const StringView& other) const;
		bool operator!=(const StringView& other) const;
		const T& operator[](size_t index) const;
	};
}
#include "StringView.hpp"