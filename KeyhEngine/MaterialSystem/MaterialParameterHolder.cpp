#include "MaterialSystemPch.h"
#include "MaterialParameterHolder.h"

namespace keyh
{
	//bool IMaterialParameterHolder::isSame(const IMaterialParameterHolder* materialParameteHolder)
	//{
	//	if (this->getType() != materialParameteHolder->getType())
	//	{
	//		KEYH_ASSERT_DEV(false, "MaterialParameterHolder Type Dismatch");
	//		return false;
	//	}

	//	if (this->getMemorySize() != materialParameteHolder->getMemorySize())
	//	{
	//		return false;
	//	}

	//	return memcmp(materialParameteHolder->getReadPtr(), getReadPtr(), getMemorySize()) == 0;		
	//}

	//template<size_t BitCount>
	//MaterialParameterType MaterialParameterHolder_BitFlag<BitCount>::getType() const
	//{
	//	if constexpr (BitCount == 8)
	//	{
	//		return MaterialParameterType::BitFlag8;
	//	}
	//	else if constexpr (BitCount == 16)
	//	{
	//		return MaterialParameterType::BitFlag16;
	//	}
	//	else if constexpr (BitCount == 32)
	//	{
	//		return MaterialParameterType::BitFlag32;
	//	}
	//}
	//
	//template<size_t BitCount>
	//void MaterialParameterHolder_BitFlag<BitCount>::setValue(uint8 index, bool flag)
	//{
	//	const size_t byteIndex = index / 8;
	//	const uint8 bitOffset = index % 8;

	//	if (byteIndex < Size)
	//	{
	//		if (flag)
	//		{
	//			_bitFlags[byteIndex] |= static_cast<uint8>(1 << bitOffset);
	//		}
	//		else
	//		{
	//			_bitFlags[byteIndex] &= static_cast<uint8>(~(1 << bitOffset));
	//		}
	//	}
	//	else
	//	{
	//		KEYH_ASSERT_DEV(false, "Index out of range");
	//	}
	//}

	//template<size_t BitCount>
	//bool MaterialParameterHolder_BitFlag<BitCount>::getValue(uint8 index) const
	//{
	//	const size_t byteIndex = index / 8;
	//	const uint8 bitOffset = index % 8;

	//	if (byteIndex < Size)
	//	{
	//		return (_bitFlags[byteIndex] & static_cast<uint8>(1 << bitOffset)) != 0;
	//	}
	//	else
	//	{
	//		KEYH_ASSERT_DEV(false, "Index out of range");
	//	}

	//	return false;
	//}

	//template class MaterialParameterHolder_BitFlag<8>;
	//template class MaterialParameterHolder_BitFlag<16>;
	//template class MaterialParameterHolder_BitFlag<32>;
}