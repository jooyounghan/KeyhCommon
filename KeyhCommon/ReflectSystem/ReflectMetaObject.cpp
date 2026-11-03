#include "ReflectSystemPch.h"
#include "ReflectMetaObject.h"

namespace keyh
{
	ReflectMetaObject ReflectMetaObject::clone() const
	{
		ReflectMetaObject result;
		for (const IReflectProperty* property : _properties)
		{
			KEYH_ASSERT(property != nullptr, "Reflected metadata cannot contain a null property.");
			if (property == nullptr)
				continue;
			IReflectProperty* copy = result._properties.push_back(property->clone());
			result._propertyMap.insert(copy->getPropertyName(), copy);
		}
		return result;
	}

	const IReflectProperty* ReflectMetaObject::findProperty(const FlyweightStringA& propertyName) const
	{
		using FindResult = HashMap<FlyweightStringA, IReflectProperty*>::ConstFindResult;
		FindResult findResult = _propertyMap.find(propertyName);

		return findResult.isFound() ? *findResult.value() : nullptr;
	}
}
