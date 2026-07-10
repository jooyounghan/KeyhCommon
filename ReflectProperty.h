#pragma once
#include "IReflectProperty.h"
#include "ReflectPropertyPolicy.h"

namespace keyh
{
	template<typename ObjectType, typename ValueType>
	class ReflectProperty : public IReflectProperty
	{
	public:
		ReflectProperty(
			const FlyweightStringA& propertyName
			, const FlyweightStringA& groupName
			, const ValueType& defaultValue
			, ReflectRefGetter<ObjectType, ValueType> refGetter
			, ReflectConstGetter<ObjectType, ValueType> constGetter
		);

	protected:
		ValueType									_defaultValue;
		ReflectRefGetter<ObjectType, ValueType>		_refGetter = nullptr;
		ReflectConstGetter<ObjectType, ValueType>	_constGetter = nullptr;

	public:
		ValueType& getValueRef(IReflectObject* object) const;
		const ValueType& getValueConstRef(const IReflectObject* object) const;

	public:
		virtual bool isDefault(const IReflectObject* object) const override;
		virtual bool isEqual(const IReflectObject* objectA, const IReflectObject* objectB) const override;

	public:
		virtual void serializeToJson(IBuffer* buffer, const IReflectObject* object) override;
		virtual void deserializeFromJson(const JsonElement& jsonElement, IReflectObject* object) override;
		virtual void serializeToBinary(IBuffer* buffer, const IReflectObject* object) override;
		virtual void deserializeFromBinary(const void* data, size_t size, IReflectObject* object) override;
	};
}
#include "ReflectProperty.hpp"

