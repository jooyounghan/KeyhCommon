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
// It only *declares* the default constructor and the destructor, so members
// with forward-declared types (e.g. Ptr<IncompleteType>) do not need to be
// complete inside the header.
// Usage:  class REFLECTIVE(MyClass) { KEYH_REFLECT_DECLARE_BODY(MyClass) ... };
#define KEYH_REFLECT_DECLARE_BODY(className) \
    friend class keyh::ReflectObject<className>;	\
	public:	\
		className();	\
		virtual ~className() override;	\

// Place this macro once in the matching .cpp file (inside the same namespace as
// the class) to define what KEYH_REFLECT_DECLARE_BODY declared.  The .cpp must
// include every type used by the class's members so that their destructors can
// be instantiated with complete types.
// Usage (MyClass.cpp):  KEYH_REFLECT_DEFINE_BODY(MyClass)
#define KEYH_REFLECT_DEFINE_BODY(className) \
	className::className() : keyh::ReflectObject<className>(#className) {}	\
	className::~className() = default;	\

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