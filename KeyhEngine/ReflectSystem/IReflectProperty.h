#pragma once
#include "ReflectionUtil.h"
#include "ReflectSerializer.h"
#include "FlyweightString.h"
#include "IBuffer.h"

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
