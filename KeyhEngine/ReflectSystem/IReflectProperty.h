#pragma once
#include "FlyweightString.h"
#include "../CommonBase/IBuffer.h"

namespace keyh
{
	class JsonElement;

	class IReflectProperty
	{
	public:
		IReflectProperty(const FlyweightStringA& name) : _name(name) {}
		virtual ~IReflectProperty() = default;

	protected:
		FlyweightStringA _name;

	public:
		inline const FlyweightStringA& getName() const { return _name; }

	public:
		virtual void serializeToJson() = 0;
		virtual void deserializeFromJson(const JsonElement& jsonElement) = 0;
		virtual void serializeToBinary(IBufferBase* buffer) = 0;
		virtual void deserializeFromBinary(IBufferBase* buffer) = 0;
	};

	template<typename ObjectType, typename ValueType>
	class ReflectPropertyBase : public IReflectProperty
	{
	protected:
		typedef ValueType& (ObjectType::* RefGetter)();
		typedef const ValueType& (ObjectType::* ConstGetter)() const;

	public:
		ReflectPropertyBase(const FlyweightStringA& name, RefGetter refGetter, ConstGetter constGetter)
			: IReflectProperty(name), _refGetter(refGetter), _constGetter(constGetter)
		{
		}

	private:
		RefGetter _refGetter = nullptr;
		ConstGetter _constGetter = nullptr;
	};
}
