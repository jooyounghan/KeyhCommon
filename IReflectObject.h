#pragma once

namespace keyh
{
	class ReflectMetaObject;
	class IReflectProperty;

	class IReflectObject
	{
	public:
		IReflectObject(const FlyweightStringA& objectName)
			: _objectName(objectName) {}
		virtual ~IReflectObject() = default;

	protected:
		FlyweightStringA _objectName;

	protected:
		virtual const ReflectMetaObject& getMetaObject() const = 0;
	
	public:
		const IReflectProperty* findProperty(const FlyweightStringA& propertyName) const;
		const OwnerVector<IReflectProperty>& getReflectProperties() const;

	public:
		inline const FlyweightStringA& getObjectName() const { return _objectName; }
		
	public:
		bool isEqual(const IReflectObject* other) const;
	};
}