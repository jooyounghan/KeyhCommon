namespace keyh
{
	template<typename T>
	size_t StrUtil::strlen(const T* str)
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
	}

	template<typename T>
	int StrUtil::strcmp(const T* lhs, const T* rhs)
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED();
	}

	template<>
	inline size_t StrUtil::strlen(const char* str)
	{
		return ::strlen(str);
	}

	template<>
	inline size_t StrUtil::strlen(const wchar_t* str)
	{
		return ::wcslen(str);
	}

	template<>
	inline int StrUtil::strcmp(const char* lhs, const char* rhs)
	{
		return ::strcmp(lhs, rhs);
	}

	template<>
	inline int StrUtil::strcmp(const wchar_t* lhs, const wchar_t* rhs)
	{
		return ::wcscmp(lhs, rhs);
	}
}