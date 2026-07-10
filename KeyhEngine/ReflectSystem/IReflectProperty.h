#pragma once
#include "ReflectionUtil.h"
#include "ReflectSerializer.h"

// Marker macro placed before a member variable to register it as a reflected property.
// Accepts optional key=value pairs parsed by the reflection code generator:
//   PropertyName = "CustomName"   override the property name (default: variable name)
//   Default      = <expr>         default value expression (default: value-initialised)
// Example:
//   KEYH_REFLECT_PROPERTY(PropertyName = "Health", Default = 100)
//   int _health = 100;
#define KEYH_REFLECT_PROPERTY(...)

// Begin a named property group.  All KEYH_REFLECT_PROPERTY declarations between
// this macro and the matching KEYH_REFLECT_PROPERTY_GROUP_END are assigned to the
// group whose name is the string literal argument.
// Example:
//   KEYH_REFLECT_PROPERTY_GROUP("Physics")
//   KEYH_REFLECT_PROPERTY()
//   float _mass = 1.0f;
//   KEYH_REFLECT_PROPERTY_GROUP_END
#define KEYH_REFLECT_PROPERTY_GROUP(groupName)

// End the current property group.  Properties after this macro (or at the top
// of the class before any group) are assigned FlyweightStringA::Empty as their
// group name.
#define KEYH_REFLECT_PROPERTY_GROUP_END

namespace keyh
{
	class IReflectObject;
	class JsonElement;

	class IReflectProperty
	{
	public:
		IReflectProperty(const FlyweightStringA& propertyName, const FlyweightStringA& groupName)
			: _propertyName(propertyName), _groupName(groupName) {}
		virtual ~IReflectProperty() = default;

	protected:
		FlyweightStringA _propertyName;
		FlyweightStringA _groupName;

	public:
		inline const FlyweightStringA& getPropertyName() const { return _propertyName; }
		inline const FlyweightStringA& getGroupName() const { return _groupName; }

	public:
		virtual bool isDefault(const IReflectObject* object) const = 0;
		virtual bool isEqual(const IReflectObject* objectA, const IReflectObject* objectB) const = 0;

	public:
		virtual void serializeToJson(IBuffer* buffer, const IReflectObject* object) = 0;
		virtual void deserializeFromJson(const JsonElement& jsonElement, IReflectObject* object) = 0;
		virtual void serializeToBinary(IBuffer* buffer, const IReflectObject* object) = 0;
		virtual void deserializeFromBinary(const void* data, size_t size, IReflectObject* object) = 0;
	};
}
