namespace keyh
{
	template<typename T, size_t Size>
	void StaticBuffer<T, Size>::writeAscii(StaticBuffer* buffer, const char* value)
	{
		while (*value != '\0')
			buffer->writeOne(static_cast<T>(*value++));
	}

	template<typename T, size_t Size>
	template<typename ValueType>
	void StaticBuffer<T, Size>::writeFormatValue(StaticBuffer* buffer, T specifier, ValueType value)
	{
		if constexpr (IsConvertible_v<ValueType, const T*>)
		{
			KEYH_ASSERT(specifier == static_cast<T>('s'), "String format argument requires %s.");
			const T* stringValue = value;
			KEYH_ASSERT(stringValue != nullptr, "String format argument must not be null.");
			if (stringValue != nullptr)
				while (*stringValue != T()) buffer->writeOne(*stringValue++);
		}
		else
		{
			KEYH_ASSERT(specifier == static_cast<T>('c'), "Character format argument requires %c.");
			buffer->writeOne(static_cast<T>(value));
		}
	}

	template<typename T, size_t Size>
	template<typename ValueType>
	void StaticBuffer<T, Size>::writeSignedFormatValue(StaticBuffer* buffer, T specifier, ValueType value)
	{
		KEYH_ASSERT(specifier == static_cast<T>('d') || specifier == static_cast<T>('i'), "Signed integer requires %d or %i.");
		const int64 signedValue = static_cast<int64>(value);
		const bool negative = signedValue < 0;
		const uint64 magnitude = negative ? static_cast<uint64>(-(signedValue + 1)) + 1 : static_cast<uint64>(signedValue);
		StaticBufferA<32> temp;
		StrUtil::intToStr(negative, magnitude, &temp);
		writeAscii(buffer, temp.getBuffer());
	}

	template<typename T, size_t Size>
	template<typename ValueType>
	void StaticBuffer<T, Size>::writeUnsignedFormatValue(StaticBuffer* buffer, T specifier, ValueType value)
	{
		KEYH_ASSERT(specifier == static_cast<T>('u') || specifier == static_cast<T>('x') || specifier == static_cast<T>('X'), "Unsigned integer requires %u, %x, or %X.");
		const uint64 unsignedValue = static_cast<uint64>(value);
		if (specifier == static_cast<T>('u'))
		{
			StaticBufferA<32> temp;
			StrUtil::intToStr(false, unsignedValue, &temp);
			writeAscii(buffer, temp.getBuffer());
			return;
		}
		const char* digits = specifier == static_cast<T>('X') ? "0123456789ABCDEF" : "0123456789abcdef";
		char reversed[16] = {};
		size_t count = 0;
		uint64 remaining = unsignedValue;
		do
		{
			reversed[count++] = digits[remaining & 0xF];
			remaining >>= 4;
		} while (remaining != 0);
		while (count > 0) buffer->writeOne(static_cast<T>(reversed[--count]));
	}

	template<typename T, size_t Size>
	template<typename ValueType>
	void StaticBuffer<T, Size>::writeFloatFormatValue(StaticBuffer* buffer, T specifier, ValueType value)
	{
		KEYH_ASSERT(specifier == static_cast<T>('f'), "Floating point value requires %f.");
		StaticBufferA<64> temp;
		StrUtil::floatToStr(static_cast<double>(value), &temp);
		writeAscii(buffer, temp.getBuffer());
	}

	template<typename T, size_t Size>
	void StaticBuffer<T, Size>::formatImpl(StaticBuffer* buffer, const T* formatString)
	{
		while (*formatString != T())
		{
			if (*formatString == static_cast<T>('%') && formatString[1] == static_cast<T>('%'))
			{
				buffer->writeOne(static_cast<T>('%'));
				formatString += 2;
			}
			else buffer->writeOne(*formatString++);
		}
	}

	template<typename T, size_t Size>
	template<typename ValueType, typename... Args>
	void StaticBuffer<T, Size>::formatImpl(StaticBuffer* buffer, const T* formatString, ValueType value, Args... args)
	{
		while (*formatString != T())
		{
			if (*formatString == static_cast<T>('%'))
			{
				if (formatString[1] == static_cast<T>('%'))
				{
					buffer->writeOne(static_cast<T>('%'));
					formatString += 2;
					continue;
				}
				break;
			}
			buffer->writeOne(*formatString++);
		}
		KEYH_ASSERT(*formatString == static_cast<T>('%') && formatString[1] != T(), "Format string has fewer placeholders than arguments.");
		if (*formatString != static_cast<T>('%') || formatString[1] == T()) return;
		const T specifier = formatString[1];
		if constexpr (IsSame_v<RemoveConstant_t<ValueType>, float> || IsSame_v<RemoveConstant_t<ValueType>, double>)
			writeFloatFormatValue(buffer, specifier, value);
		else if constexpr (IsIntegral_v<ValueType>)
		{
			if (specifier == static_cast<T>('c'))
				buffer->writeOne(static_cast<T>(value));
			else if constexpr (IsSame_v<RemoveConstant_t<ValueType>, bool> || IsSame_v<RemoveConstant_t<ValueType>, unsigned int> || IsSame_v<RemoveConstant_t<ValueType>, unsigned long> || IsSame_v<RemoveConstant_t<ValueType>, unsigned long long> || IsSame_v<RemoveConstant_t<ValueType>, unsigned short> || IsSame_v<RemoveConstant_t<ValueType>, unsigned char>)
				writeUnsignedFormatValue(buffer, specifier, value);
			else
				writeSignedFormatValue(buffer, specifier, value);
		}
		else
			writeFormatValue(buffer, specifier, value);
		formatImpl(buffer, formatString + 2, args...);
	}

	template<typename T, size_t Size>
	template<typename... Args>
	void StaticBuffer<T, Size>::format(const T* formatString, Args... args)
	{
		KEYH_ASSERT(formatString != nullptr, "Format string must not be null.");
		if (formatString == nullptr)
			return;
		formatImpl(this, formatString, args...);
	}
}
