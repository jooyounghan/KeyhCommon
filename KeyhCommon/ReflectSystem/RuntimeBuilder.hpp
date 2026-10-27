namespace keyh
{
	template<typename RawType, typename BinaryType>
	void RuntimeBuilder::build(const RawType& rawData, BinaryType& binaryData)
	{
		if constexpr (IsSame_v<RawType, BinaryType> == false)
		{
			KEYH_STATIC_ASSERT(false, "RuntimeBuilder::build() is not implemented for the given types.");
		}
	}
}
