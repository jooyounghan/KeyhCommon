#include "ReflectSystemPch.h"
#include "IReflectObject.h"

namespace keyh
{
	bool IReflectObject::isEqual(const IReflectObject* other) const
	{
		if (this == other || other == nullptr)
			return true;

		const ReflectMetaObject& metaA = getMetaObject();
		const ReflectMetaObject& metaB = other->getMetaObject();
		if (&metaA != &metaB)
			return false;

		const OwnerVector<IReflectProperty>& reflectProperties = metaA.getReflectProperties();
		for (const IReflectProperty* property : reflectProperties)
		{
			if (property->isEqual(this, other) == false)
			{
				return false;
			}
		}

		return true;
	}
}