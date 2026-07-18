#pragma once

namespace keyh
{
	class ReflectMetaObject;

	class IReflectObject
	{
	public:
		IReflectObject(const FlyweightStringA& objectName)
			: _objectName(objectName) {}
		virtual ~IReflectObject() = default;

	protected:
		FlyweightStringA _objectName;

	public:
		virtual const ReflectMetaObject& getMetaObject() const = 0;

	public:
		inline const FlyweightStringA& getObjectName() const { return _objectName; }
		
	public:
		bool isEqual(const IReflectObject* other) const;
	};
}