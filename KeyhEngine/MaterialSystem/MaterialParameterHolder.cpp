#include "MaterialSystemPch.h"
#include "MaterialParameterHolder.h"

namespace keyh
{
	bool IMaterialParameterHolder::isSame(const IMaterialParameterHolder* materialParameteHolder)
	{
		if (this->getType() != materialParameteHolder->getType())
		{
			KEYH_ASSERT_DEV(false, "MaterialParameterHolder Type Dismatch");
			return false;
		}

		if (this->getMemorySize() != materialParameteHolder->getMemorySize())
		{
			return false;
		}

		return memcmp(materialParameteHolder->getReadPtr(), getReadPtr(), getMemorySize()) == 0;		
	}

}