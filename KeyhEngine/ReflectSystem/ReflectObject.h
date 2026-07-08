#pragma once
#include "IReflectObject.h"

// Expands to "ClassName : public keyh::ReflectObject<ClassName>".
// Usage:  class REFLECTIVE(MyClass) { ... };
#define REFLECTIVE(className) \
className : public keyh::ReflectObject<className> \

// Place this macro once inside the body of every REFLECTIVE class so that
// keyh::ReflectObject<ClassName>::initializeMetaObject() (and the lambdas
// defined within it) can access the class's protected/private members.
// Usage:  class REFLECTIVE(MyClass) { KEYH_REFLECT_BODY(MyClass) ... };
#define KEYH_REFLECT_BODY(className) \
    friend class keyh::ReflectObject<className>;

namespace keyh
{
	template<typename ObjectType>
	class ReflectObject : public IReflectObject
	{
	public:
		ReflectObject(const FlyweightStringA& objectName);

	protected:
		static ReflectMetaObject initializeMetaObject();

	public:
		virtual const ReflectMetaObject& getMetaObject() const override;
	};
}
#include "ReflectObject.hpp"