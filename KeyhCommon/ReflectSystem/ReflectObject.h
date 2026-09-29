#pragma once
#include "IReflectObject.h"
#include "ReflectMetaObject.h"

// Expands to "ClassName : public keyh::ReflectObject<ClassName>".
// Usage:  class REFLECTIVE(MyClass) { ... };
#define REFLECTIVE(className) \
className : public keyh::ReflectObject<className> \

// Single reflected inheritance; use the matching derived body macros below.
#define REFLECTIVE_DERIVED(className, baseName) className : public baseName

#define KEYH_REFLECT_DECLARE_DERIVED_BODY(className) \
    friend class keyh::ReflectObject<className>; \
    public: \
        className(); \
        const keyh::ReflectMetaObject& getMetaObject() const override;

#define KEYH_REFLECT_DEFINE_DERIVED_BODY(className, baseName) \
    className::className() { _objectName = #className; } \
    const keyh::ReflectMetaObject& className::getMetaObject() const \
    { static keyh::ReflectMetaObject metaObject = keyh::ReflectObject<className>::initializeMetaObject(); return metaObject; } \
    namespace { const bool sKeyhReflectRegistered_##className = keyh::ReflectTypeRegistry<baseName>::template registerType<className>(#className); }

// Place this macro once inside the body of every REFLECTIVE class so that
// keyh::ReflectObject<ClassName>::initializeMetaObject() (and the lambdas
// defined within it) can access the class's protected/private members.
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
		friend ObjectType;
		template<typename OtherObjectType>
		friend class ReflectObject;
		static ReflectMetaObject initializeMetaObject();

	public:
		virtual const ReflectMetaObject& getMetaObject() const override;
	};
}
#include "ReflectObject.hpp"
#include "ReflectTypeRegistry.h"
