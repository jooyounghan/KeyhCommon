#pragma once
#include "CommonCore.h"

#include "StringView.h"
#include "Vector.h"

namespace keyh
{
	class SplitterString
	{
	public:
		SplitterString() = default;
		~SplitterString() = default;

	public:
		SplitterString(const utf8* string, size_t length, const char* delimiter);

	private:
		Vector<StringViewA> _tokens;

	public:
		void split(const utf8* string, size_t length, const char* delimiter);
		void clear();

	public:
		inline Vector<StringViewA>::ConstIterator begin() const { return _tokens.begin(); }
		inline Vector<StringViewA>::ConstIterator end() const { return _tokens.end(); }

	public:
		const StringViewA& operator[](size_t index) const;

	public:
		inline size_t count() const { return _tokens.size(); }
		inline size_t size() const { return _tokens.size(); }
		inline bool empty() const { return _tokens.empty(); }
		inline const Vector<StringViewA>& getTokens() const { return _tokens; }
	};
}
