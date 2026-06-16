#include "CommonBasePch.h"
#include "StaticString.h"
#include <string>
namespace keyh
{
	template<typename T>
	StaticString<T>::~StaticString()
	{

	}
	
	template<typename T>
	StaticString<T>::StaticString(const T * str)
	{
	}
	
	template<typename T>
	StaticString<T>::StaticString(const StaticString & other)
	{
	}
	
	template<typename T>
	StaticString<T>& StaticString<T>::operator=(const StaticString & other)
	{
	}
	
	template<typename T>
	StaticString<T>::StaticString(StaticString&& other) noexcept
	{
	}

	template<typename T>
	StaticString<T>& StaticString<T>::operator=(StaticString && other) noexcept
	{
	}

	template<typename T>
	bool StaticString<T>::operator==(const StaticString& other) const
	{
		return false;
	}

	template<typename T>
	bool StaticString<T>::operator!=(const StaticString& other) const
	{
		return false;
	}

	template<typename T>
	T& StaticString<T>::operator[](size_t index)
	{

	}

	template<typename T>
	const T& StaticString<T>::operator[](size_t index) const
	{
	}

	template<typename T>
	void StaticString<T>::clear()
	{}

	template class StaticString<char>;
	template class StaticString<wchar_t>;
}