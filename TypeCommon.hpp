namespace keyh
{
	template<typename T>
	constexpr void TypeCommon::requireIntegral() noexcept
	{
		static_assert(std::is_integral_v<T>, "T must be an integral type");
	}
}