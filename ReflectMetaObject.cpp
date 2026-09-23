#include "ReflectSystemPch.h"
#include "ReflectMetaObject.h"

namespace keyh
{
	const IReflectProperty* ReflectMetaObject::findProperty(const FlyweightStringA& propertyName) const
	{
		using FindResult = HashMap<FlyweightStringA, IReflectProperty*>::ConstFindResult;
		FindResult findResult = _propertyMap.find(propertyName);

		return findResult.isFound() ? *findResult.value() : nullptr;
	}
}
