#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	class IMaterialParameterHolder
	{
	public:
		bool isSame(const IMaterialParameterHolder* materialParameterHolder);

	public:
		virtual MaterialParameterType	getType() const = 0;
		virtual const void*				getReadPtr() const = 0;
		virtual void*					getWritePtr() = 0;
		virtual size_t					getMemorySize() const = 0;
	};

	template<typename T>
	struct IsValidMaterialType
	{
		static constexpr bool Value = false;
	};

#define VALID_MATERIAL_TYPE(T) \
	template<> struct IsValidMaterialType<T> { static constexpr bool Value = true; }
	
	VALID_MATERIAL_TYPE(int);
	VALID_MATERIAL_TYPE(int2);
	VALID_MATERIAL_TYPE(float);
	VALID_MATERIAL_TYPE(float2);
	VALID_MATERIAL_TYPE(float3);
	VALID_MATERIAL_TYPE(float4);

	template<typename T>
	class MaterialParameterHolder : public IMaterialParameterHolder
	{
		static_assert(IsValidMaterialType<T>::Value, "Unsupported MaterialParameter type.");

	public:
		virtual MaterialParameterType	getType() const override;
		virtual const void*				getReadPtr() const override { return &_value; }
		virtual void*					getWritePtr() override { return &_value; }
		virtual size_t					getMemorySize() const override { return sizeof(T); }

	protected:
		T _value;

	public:
		inline void		setValue(const T& value) { _value = value; }
		inline void		setValue(T&& value) { _value = keyh::move(value); }
		inline T&		getValue() { return _value; }
		inline const T& getValue() const { return _value; }
	};

	class MaterialParamaterHolderRefencedPath_Texture : public IMaterialParameterHolder
	{
	public:
		virtual MaterialParameterType	getType() const override { return MaterialParameterType::ReferencedPath_Texture; }
		virtual const void*				getReadPtr() const override { return _path.getBuffer(); }
		virtual void*					getWritePtr() override { return _path.getBuffer(); }
		virtual size_t					getMemorySize() const override { return _path.getSizeBytes(); }

	public:
		inline void						setPath(const utf8* value, size_t size) { _path.write(value, size); }

	protected:
		StaticBuffer<utf8, kReferencePathMaxLength> _path;
		uint32										_resourceAllocatedIndex = 0;
	};
}

#include "MaterialParameterHolder.hpp"