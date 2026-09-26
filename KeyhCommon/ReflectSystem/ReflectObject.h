#pragma once
#include "IReflectObject.h"
#include "ReflectMetaObject.h"

// Expands to "ClassName : public keyh::ReflectObject<ClassName>".
// Usage:  class REFLECTIVE(MyClass) { ... };
#define REFLECTIVE(className) \
className : public keyh::ReflectObject<className> \

// Place this macro once inside the body of every REFLECTIVE class so that
// keyh::ReflectObject<ClassName>::initializeMetaObject() can access its
// private reflected members. CodeGen emits the explicit specialization in
// the generated .inl included by the matching .cpp.
// It only *declares* the default constructor, so members with forward-declared
// types (e.g. Ptr<IncompleteType>) do not need to be complete inside the header.
// The destructor is intentionally not handled by this macro; declare and define
// it yourself when the class needs one.
// Usage:  class REFLECTIVE(MyClass) { KEYH_REFLECT_DECLARE_BODY(MyClass) ... };
#define KEYH_REFLECT_DECLARE_BODY(className) \
    friend class keyh::ReflectObject<className>;	\
	public:	\
		className();	\

// Place this macro once in the matching .cpp file (inside the same namespace as
// the class) to define what KEYH_REFLECT_DECLARE_BODY declared.
// Usage (MyClass.cpp):  KEYH_REFLECT_DEFINE_BODY(MyClass)
#define KEYH_REFLECT_DEFINE_BODY(className) \
	className::className() : keyh::ReflectObject<className>(#className) {}	\

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
