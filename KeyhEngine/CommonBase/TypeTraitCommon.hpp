namespace keyh
{
	template<typename T>
	constexpr void TypeTrait::requireIntegral() noexcept
	{
		static_assert(IsIntegral<T>, "T must be an integral type");
	}
}