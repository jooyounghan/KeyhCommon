#include "ReflectObject.h"
namespace keyh
{
	template<typename ObjectType>
	ReflectObject<ObjectType>::ReflectObject(const FlyweightStringA& objectName)
		: IReflectObject(objectName) 
	{
	}

	template<typename ObjectType>
	ReflectMetaObject ReflectObject<ObjectType>::initializeMetaObject()
	{
		STATIC_ASSERT_FUNCTION_NOT_SUPPORTED(ReflectMetaObject<ObjectType>);
	}

	template<typename ObjectType>
	const ReflectMetaObject& keyh::ReflectObject<ObjectType>::getMetaObject() const
	{
		static ReflectMetaObject metaObject = initializeMetaObject();
		return metaObject;
	}
}