#include "CommonBasePch.h"
#include "SplitterString.h"

namespace keyh
{
	template<typename T>
	SplitterString<T>::SplitterString(const StringView<T>& string, const char* delimiter)
	{
		split(string, delimiter);
	}

	template<typename T>
	void SplitterString<T>::split(const StringView<T>& string, const char* delimiter)
	{
		clear();
		const size_t length = string.length();
		if (string.data() == nullptr || length == 0)
		{
			return;
		}

		const size_t delimiterLength = delimiter != nullptr ? StrUtil::strlen(delimiter) : 0;
		if (delimiterLength == 0 || delimiterLength > length)
		{
			StringView<T> trimmedString(string.data(), length);
			trimmedString.trim();
			_tokens.emplace_back(trimmedString);
			return;
		}

		size_t tokenBegin = 0;
		const T* stringData = string.data();
		for (size_t offset = 0; offset + delimiterLength <= length;)
		{
			if (memcmp(stringData + offset, delimiter, delimiterLength * sizeof(T)) != 0)
			{
				++offset;
				continue;
			}

			StringView<T> token(stringData + tokenBegin, offset - tokenBegin);
			token.trim();
			_tokens.emplace_back(token);

			offset += delimiterLength;
			tokenBegin = offset;
		}

		StringView<T> token(stringData + tokenBegin, length - tokenBegin);
		token.trim();
		_tokens.emplace_back(token);
	}

	template<typename T>
	void SplitterString<T>::clear()
	{
		_tokens.clear();
	}

	template<typename T>
	const StringView<T>& SplitterString<T>::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _tokens.size(), "Index out of bounds");
		return _tokens[index];
	}

	template class SplitterString<char>;
	template class SplitterString<wchar_t>;

}
