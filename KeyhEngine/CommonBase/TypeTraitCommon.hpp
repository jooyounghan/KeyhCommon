namespace keyh
{
	template<typename T>
	constexpr void TypeTrait::requireIntegral() noexcept
	{
		static_assert(IsIntegral_v<T>, "T must be an integral type");
	}

	template<typename Derived, typename Base>
	constexpr void TypeTrait::requireDerivedFrom() noexcept
	{
		static_assert(IsDerivedFrom_v<Derived, Base>, "Derived must be derived from Base");
	}
}