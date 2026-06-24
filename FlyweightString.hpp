namespace keyh
{
	template<typename T, typename Hasher>
	StringView<T> StringPool1<T, Hasher>::findOrInsert(const StringView<T>& str)
	{
		StaticString<T> staticStr(str.c_str(), str.length());
		_stringContainer.insert(staticStr);
		return StringView<T>();
	}

	template<typename T, typename Hasher, size_t PoolSize>
	inline StringView<T> StringPool2<T, Hasher, PoolSize>::findOrInsert(const StringView<T>& str)
	{
		return StringView<T>();
	}

	template<typename T, typename Hasher, size_t PoolSize>
	inline StringView<T> StringPool3<T, Hasher, PoolSize>::findOrInsert(const StringView<T>& str)
	{
		return StringView<T>();
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const T* str)
	{
		StringView<T> stringView(str);
		_stringView = StringPool::getInstance().findOrInsert(stringView);
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const StaticString<T>& str)
	{
		StringView<T> stringView(str.c_str(), str.length());
		_stringView = StringPool::getInstance().findOrInsert(stringView);
	}

	template<typename T, typename Hasher, typename StringPool>
	FlyweightString<T, Hasher, StringPool>::FlyweightString(const StringView<T>& str)
	{
		_stringView = StringPool::getInstance().findOrInsert(str);
	}
}