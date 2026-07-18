#pragma once
#include "IReflectObject.h"
#include "ReflectMetaObject.h"

// Expands to "ClassName : public keyh::ReflectObject<ClassName>".
// Usage:  class REFLECTIVE(MyClass) { ... };
#define REFLECTIVE(className) \
className : public keyh::ReflectObject<className> \

// Place this macro once inside the body of every REFLECTIVE class so that
// keyh::ReflectObject<ClassName>::initializeMetaObject() (and the lambdas
// defined within it) can access the class's protected/private members.
// Usage:  class REFLECTIVE(MyClass) { KEYH_REFLECT_BODY(MyClass) ... };
#define KEYH_REFLECT_BODY(className) \
    friend class keyh::ReflectObject<className>;	\
	public:	\
		className() : keyh::ReflectObject<className>(#className) {}	\

namespace keyh
{
	template<typename ObjectType>
	class ReflectObject : public IReflectObject
	{
	public:
		ReflectObject(const FlyweightStringA& objectName);
		virtual ~ReflectObject() override = default;

	protected:
		static ReflectMetaObject initializeMetaObject();

	public:
		virtual const ReflectMetaObject& getMetaObject() const override;
	};
}
#include "ReflectObject.hpp"