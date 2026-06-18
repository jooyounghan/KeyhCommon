#pragma once

namespace keyh
{
	struct TypeTrait
	{
		template<typename T>
		static constexpr void requireIntegral() noexcept;

		template<typename Derived, typename Base>
		static constexpr void requireDerivedFrom() noexcept;
	};

	typedef decltype(nullptr) Nullptr_t;

	template <typename T>
	T DeclVal() noexcept;

#pragma region IsSame
	template <typename T, typename U>
	struct IsSameImpl
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct IsSameImpl<T, T>
	{
		static constexpr bool value = true;
	};

	template <typename T, typename U>
	constexpr bool IsSame_v = IsSameImpl<T, U>::value;
#pragma endregion

#pragma region IsClass
	template <typename T>
	struct IsClassHelper
	{
	private:
		template <typename C>
		static char test(int C::*);

		template <typename C>
		static long long test(...);

	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template <typename T>
	constexpr bool IsClass_v = IsClassHelper<T>::value;
#pragma endregion

#pragma region IsDerivedFrom
	template <typename Derived, typename Base>
	struct IsDerivedFromHelper
	{
	private:
		static void testFunc(Base*);
		template <typename D, typename = decltype(testFunc(DeclVal<D*>()))>
		static char test(int);
		template <typename D>
		static long long test(...);
	public:
		static constexpr bool value = sizeof(test<Derived>(0)) == sizeof(char);
	};

	template <typename Derived, typename Base, bool = IsClass_v<Derived> && IsClass_v<Base>>
	struct IsDerivedFromImpl
	{
		static constexpr bool value = IsDerivedFromHelper<Derived, Base>::value;
	};

	template <typename Derived, typename Base>
	struct IsDerivedFromImpl<Derived, Base, false>
	{
		static constexpr bool value = IsSame_v<Derived, Base>;
	};

	template <typename Derived, typename Base>
	constexpr bool IsDerivedFrom_v = IsDerivedFromImpl<Derived, Base>::value;
#pragma endregion

#pragma region TypeIdentity
	template <typename T>
	struct TypeIdentityImpl
	{
		using type = T;
	};

	template <typename T>
	using TypeIdentity_t = typename TypeIdentityImpl<T>::type;
#pragma endregion

#pragma region RemoveReference
	template <typename T>
	struct RemoveReferenceImpl { using type = T; };

	template <typename T>
	struct RemoveReferenceImpl<T&> { using type = T; };

	template <typename T>
	struct RemoveReferenceImpl<T&&> { using type = T; };

	template <typename T>
	using RemoveReference_t = typename RemoveReferenceImpl<T>::type;
#pragma endregion

#pragma region RemoveConstant
	template <typename T>
	struct RemoveConstantImpl { using type = T; };

	template <typename T>
	struct RemoveConstantImpl<const T> { using type = T; };

	template <typename T>
	struct RemoveConstantImpl<volatile T> { using type = T; };

	template <typename T>
	struct RemoveConstantImpl<const volatile T> { using type = T; };

	template <typename T>
	using RemoveConstant_t = typename RemoveConstantImpl<T>::type;
#pragma endregion

#pragma region IsLvalueReference
	template <typename T>
	struct IsLvalueReferenceImpl
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct IsLvalueReferenceImpl<T&>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsLvalueReference_v = IsLvalueReferenceImpl<T>::value;
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
	constexpr bool IsConvertible_v = IsConvertibleImpl<From, To>::value;
#pragma endregion

#pragma region IsIntegral
	template <typename T>
	struct IsIntegralHelper
	{
		static constexpr bool value = false;
	};

	template <> struct IsIntegralHelper<bool> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<char> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<signed char> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<unsigned char> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<wchar_t> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<char16_t> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<char32_t> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<short> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<unsigned short> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<int> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<unsigned int> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<long> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<unsigned long> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<long long> { static constexpr bool value = true; };
	template <> struct IsIntegralHelper<unsigned long long> { static constexpr bool value = true; };

	template <typename T>
	struct IsIntegralImpl
	{
		static constexpr bool value = IsIntegralHelper<RemoveConstant_t<T>>::value;
	};

	template <typename T>
	constexpr bool IsIntegral_v = IsIntegralImpl<T>::value;
#pragma endregion

#pragma region IsArray
	template <typename T>
	struct IsArrayImpl
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct IsArrayImpl<T[]>
	{
		static constexpr bool value = true;
	};

	template <typename T, unsigned long long N>
	struct IsArrayImpl<T[N]>
	{
		static constexpr bool value = true;
	};

	template <typename T>
	constexpr bool IsArray_v = IsArrayImpl<RemoveReference_t<T>>::value;
#pragma endregion

#pragma region IsFunction
	template <typename T>
	struct IsFunctionImpl
	{
		static constexpr bool value = false;
	};

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...)> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...)&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...)&&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const&&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile&&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile&&> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) & noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const& noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile& noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile& noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) && noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const&& noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) volatile&& noexcept> { static constexpr bool value = true; };

	template <typename Ret, typename... Args>
	struct IsFunctionImpl<Ret(Args...) const volatile&& noexcept> { static constexpr bool value = true; };

	template <typename T>
	constexpr bool IsFunction_v = IsFunctionImpl<RemoveReference_t<T>>::value;
#pragma endregion

#pragma region Decay
	template <typename T, bool IsArrayCase, bool IsFunctionCase>
	struct DecaySelector;

	template <typename T>
	struct DecaySelector<T, false, false>
	{
		using type = RemoveConstant_t<T>;
	};

	template <typename T>
	struct DecaySelector<T, true, false>
	{
		using type = RemoveConstant_t<T>*;
	};

	template <typename T>
	struct DecaySelector<T, false, true>
	{
		using type = T*;
	};

	template <typename T>
	struct DecayImpl
	{
		using U = RemoveReference_t<T>;
		using type = typename DecaySelector<U, IsArrayImpl<U>::value, IsFunctionImpl<U>::value>::type;
	};

	template <typename T>
	using Decay_t = typename DecayImpl<T>::type;
#pragma endregion

#pragma region Move
	template <typename T>
	constexpr RemoveReference_t<T>&& move(T&& arg) noexcept
	{
		return static_cast<RemoveReference_t<T>&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(RemoveReference_t<T>& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}

	template <typename T>
	constexpr T&& forward(RemoveReference_t<T>&& arg) noexcept
	{
		static_assert(!IsLvalueReference_v<T>, "bad forward");
		return static_cast<T&&>(arg);
	}
#pragma endregion
}

#include "TypeTraitCommon.hpp"