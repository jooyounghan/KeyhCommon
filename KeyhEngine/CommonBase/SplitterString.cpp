#include "CommonBasePch.h"
#include "SplitterString.h"

namespace keyh
{
	SplitterString::SplitterString(const utf8* string, size_t length, const char* delimiter)
	{
		split(string, length, delimiter);
	}

	void SplitterString::split(const utf8* string, size_t length, const char* delimiter)
	{
		clear();

		if (string == nullptr || length == 0)
			return;

		const char* begin = reinterpret_cast<const char*>(string);
		const size_t delimiterLength = delimiter != nullptr ? StrUtil::strlen(delimiter) : 0;

		if (delimiterLength == 0 || delimiterLength > length)
		{
			_tokens.emplace_back(begin, length);
			return;
		}

		size_t tokenBegin = 0;
		for (size_t offset = 0; offset + delimiterLength <= length;)
		{
			if (std::memcmp(begin + offset, delimiter, delimiterLength) != 0)
			{
				++offset;
				continue;
			}

			_tokens.emplace_back(begin + tokenBegin, offset - tokenBegin);
			offset += delimiterLength;
			tokenBegin = offset;
		}

		_tokens.emplace_back(begin + tokenBegin, length - tokenBegin);
	}

	void SplitterString::clear()
	{
		_tokens.clear();
	}

	const StringViewA& SplitterString::operator[](size_t index) const
	{
		KEYH_ASSERT(index < _tokens.size(), "Index out of bounds");
		return _tokens[index];
	}
}
