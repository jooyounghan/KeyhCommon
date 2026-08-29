#pragma once
#include "CommonCore.h"

#include "StringView.h"
#include "Vector.h"

namespace keyh
{
	template<typename T>
	class SplitterString
	{
	public:
		SplitterString() = default;
		~SplitterString() = default;

	public:
		SplitterString(const StringView<T>& string, const char* delimiter);

	private:
		Vector<StringView<T>> _tokens;

	private:
		void split(const StringView<T>& string, const char* delimiter);
		void clear();

	public:
		inline Vector<StringView<T>>::ConstIterator begin() const { return _tokens.begin(); }
		inline Vector<StringView<T>>::ConstIterator end() const { return _tokens.end(); }

	public:
		const StringView<T>& operator[](size_t index) const;

	public:
		inline size_t size() const { return _tokens.size(); }
		inline bool empty() const { return _tokens.empty(); }
		inline const Vector<StringView<T>>& getTokens() const { return _tokens; }
	};

	using SplitterStringA = SplitterString<char>;
	using SplitterStringW = SplitterString<wchar_t>;
}
