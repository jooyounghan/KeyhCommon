#include "ReflectSystemPch.h"
#include "ReflectMetaObject.h"

namespace keyh
{
	IReflectProperty* ReflectMetaObject::findProperty(const FlyweightStringA& propertyName)
	{
		using FindResult = HashMap<FlyweightStringA, IReflectProperty*>::FindResult;
		FindResult findResult = _propertyMap.find(propertyName);

		return findResult.isFound() ? findResult.value() : nullptr;
	}
}
