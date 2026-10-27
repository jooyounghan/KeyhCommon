#pragma once
namespace keyh
{
	struct RuntimeBuilder
	{
		template<typename RawType, typename BinaryType>
		static void build(const RawType& rawData, BinaryType& binaryData);
	};
}

#include "RuntimeBuilder.hpp"