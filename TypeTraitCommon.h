#pragma once

namespace keyh 
{
	struct TypeTrait
	{
		template<typename T>
		static constexpr void requireIntegral() noexcept;
	};

	template <typename T>
	T DeclVal() noexcept;

#pragma region TypeIdentity
	template <typename T>
	struct TypeIdentityImpl
	{
		using type = T;
	};

	template <typename T>
	using TypeIdentity = typename TypeIdentityImpl<T>::type;
#pragma endregion

#pragma region RemoveReference
	template <typename T>
	struct RemoveReference { using type = T; };

	template <typename T>
	struct RemoveReference<T&> { using type = T; };

	template <typename T>
	struct RemoveReference<T&&> { using type = T; };
#pragma endregion

#pragma region RemoveConstant
	template <typename T>
	struct RemoveConstant
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveConstant<const T>
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveConstant<volatile T>
	{
		typedef T Type;
	};

	template <typename T>
	struct RemoveConstant<const volatile T>
	{
		typedef T Type;
	};
#pragma endregion

#pragma region IsLvalueReference
	template <typename T>
	struct IsLvalueReferenceImpl
	{
		static const bool value = false;
	};

	template <typename T>
	struct IsLvalueReferenceImpl<T&>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsLvalueReference = IsLvalueReferenceImpl<T>::value;
#pragma endregion

#pragma region IsConvertibleImpl
	template <typename From, typename To>
	struct IsConvertibleHelper
	{
	private:
		static void testFunc(To);

		template <typename F, typename = decltype(testFunc(DeclVal<F>()))>
		static char test(int);

		template <typename F>
		static long long test(...);

	public:
		static constexpr bool value = sizeof(test<From>(0)) == sizeof(char);
	};

	template <typename From, typename To>
	struct IsConvertibleImpl
	{
		static constexpr bool value = IsConvertibleHelper<From, To>::value;
	};

	template <typename From, typename To>
	constexpr bool IsConvertible = IsConvertibleImpl<From, To>::value;
#pragma endregion

#pragma region IsIntegral
	template <typename T>
	struct IsIntegralHelper
	{
		static const bool value = false;
	};

	template <> struct IsIntegralHelper<bool> { static const bool value = true; };
	template <> struct IsIntegralHelper<char> { static const bool value = true; };
	template <> struct IsIntegralHelper<signed char> { static const bool value = true; };
	template <> struct IsIntegralHelper<unsigned char> { static const bool value = true; };
	template <> struct IsIntegralHelper<wchar_t> { static const bool value = true; };
	template <> struct IsIntegralHelper<char16_t> { static const bool value = true; };
	template <> struct IsIntegralHelper<char32_t> { static const bool value = true; };
	template <> struct IsIntegralHelper<short> { static const bool value = true; };
	template <> struct IsIntegralHelper<unsigned short> { static const bool value = true; };
	template <> struct IsIntegralHelper<int> { static const bool value = true; };
	template <> struct IsIntegralHelper<unsigned int> { static const bool value = true; };
	template <> struct IsIntegralHelper<long> { static const bool value = true; };
	template <> struct IsIntegralHelper<unsigned long> { static const bool value = true; };
	template <> struct IsIntegralHelper<long long> { static const bool value = true; };
	template <> struct IsIntegralHelper<unsigned long long> { static const bool value = true; };

	template <typename T>
	struct IsIntegralImpl
	{
		static const bool value = IsIntegralHelper<typename RemoveConstant<T>::Type>::value;
	};

	template <typename T>
	const bool IsIntegral = IsIntegralImpl<T>::value;
#pragma endregion

#pragma region IsArray
	template <typename T>
	struct IsArrayImpl
	{
		static const bool value = false;
	};

	template <typename T>
	struct IsArrayImpl<T[]>
	{
		static const bool value = true;
	};

	template <typename T, unsigned long long N>
	struct IsArrayImpl<T[N]>
	{
		static const bool value = true;
	};

	template <typename T>
	constexpr bool IsArray = IsArrayImpl<typename RemoveReference<T>::type>::value;
#pragma endregion

#pragma region IsFunction
	template <typename T>
	struct IsFunctionImp
	{
		static const bool value = false;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...)>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...)&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...)&&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const&&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile&&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile&&>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) & noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const& noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile& noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile& noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) && noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const&& noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) volatile&& noexcept>
	{
		static const bool value = true;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImp<Ret(Args...) const volatile&& noexcept>
	{
		static const bool value = true;
	};

	template <typename T>
	constexpr bool IsFunction = IsFunctionImp<typename RemoveReference<T>::type>::value;
#pragma endregion

#pragma region Decay
	template <typename T, bool IsArrayCase, bool IsFunctionCase>
	struct DecaySelector;

	template <typename T>
	struct DecaySelector<T, false, false>
	{
		typedef typename RemoveConstant<T>::Type Type;
	};

	template <typename T>
	struct DecaySelector<T, true, false>
	{
		typedef typename RemoveConstant<T>::Type* Type;
	};

	template <typename T>
	struct DecaySelector<T, false, true>
	{
		typedef T* Type;
	};

	template <typename T>
	struct DecayImpl
	{
		typedef typename RemoveReference<T>::type U;
		typedef typename DecaySelector<U, IsArrayImpl<U>::value, IsFunctionImp<U>::value>::Type Type;
	};

	template <typename T>
	using Decay = typename DecayImpl<T>::Type;
#pragma endregion

#pragma region Move
	template <typename T>
	constexpr typename RemoveReference<T>::type&& move(T&& arg) noexcept
	{
		return static_cast<typename RemoveReference<T>::type&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(typename RemoveReference<T>::type& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(typename RemoveReference<T>::type&& arg) noexcept
	{
		static_assert(!IsLvalueReference<T>, "bad forward");
		return static_cast<T&&>(arg);
	}
#pragma endregion
}

#include "TypeTraitCommon.hpp"