#pragma once
#include "MaterialEnum.h"

namespace keyh
{
	/*
	* MaterialParameterView는 실제 데이터를 가지고 있는 형태가 아니라
	* 데이터에 대한 View를 지원하는 Interface이다.
	* MaterialParameter Holder는 MaterialParameter View를 상속받으며, 
	* Holder가 실제 데이터를 가지고 있고 해당 데이터를 MaterialParameter View를 통해 View로 제공한다. 
	* 개발용 데이터는 Holder를 사용하고, 
	* Release용 데이터는 MaterialParameterView를 관리하는 Material에 대한 View로 구성된 Reference를 사용한다.
	* 
	* Shader Cache : 매터리얼을 빌드한 결과를 캐싱하는 바이너리 파일
	* MaterialParameter Cache : 매터리얼 파라미터에 대한 결과를 캐싱하는 바이너리 파일
	* Shader Cache + MaterialParameter Cache = Material Cache
	* 
	* Material.hlsl, Material.material은 개발용 데이터로,
	* MaterialCache를 먼저 읽고, 이가 없을 경우 개발용 데이터를 직접 읽어서 
	* MaterialCache를 생성하게 끔 처리한다.
	* Material.hlsl를 읽어서 Shader Cache를 생성하고, 
	* Material.material을 읽어서 MaterialParameter Cache를 생성한다.

	Material
	* 그리고 필요시 MaterialCache를 Export한다.


	*/

	/*
	외부에서 어떻게 데이터를 
	*/
	

//	class IMaterialParameterHolder
//	{
//	public:
//		virtual ~IMaterialParameterHolder() = default;
//
//	public:
//		bool isSame(const IMaterialParameterHolder* materialParameterHolder);
//
//	public:
//		virtual MaterialParameterType	getType() const = 0;
//		virtual const void*				getReadPtr() const = 0;
//		virtual void*					getWritePtr() = 0;
//		virtual size_t					getMemorySize() const = 0;
//	};
//
//	template<typename T>
//	struct IsValidMaterialType
//	{
//		static constexpr bool Value = false;
//	};
//
//#define VALID_MATERIAL_TYPE(T) \
//	template<> struct IsValidMaterialType<T> { static constexpr bool Value = true; }
//	
//	VALID_MATERIAL_TYPE(int);
//	VALID_MATERIAL_TYPE(int2);
//	VALID_MATERIAL_TYPE(float);
//	VALID_MATERIAL_TYPE(float2);
//	VALID_MATERIAL_TYPE(float3);
//	VALID_MATERIAL_TYPE(float4);
//
//	template<typename T>
//	class MaterialParameterHolder : public IMaterialParameterHolder
//	{
//		static_assert(IsValidMaterialType<T>::Value, "Unsupported MaterialParameter type.");
//
//	public:
//		MaterialParameterHolder(T defaultValue);
//
//	public:
//		virtual MaterialParameterType	getType() const override;
//		virtual const void*				getReadPtr() const override { return &_value; }
//		virtual void*					getWritePtr() override { return &_value; }
//		virtual size_t					getMemorySize() const override { return sizeof(T); }
//
//	protected:
//		T _value;
//
//	public:
//		inline void		setValue(const T& value) { _value = value; }
//		inline void		setValue(T&& value) { _value = keyh::move(value); }
//		inline T&		getValue() { return _value; }
//		inline const T& getValue() const { return _value; }
//	};
//
//	class IMaterialParameterHolder_BitFlag : public IMaterialParameterHolder
//	{
//	public:
//		virtual ~IMaterialParameterHolder_BitFlag() = default;
//
//		virtual void	setValue(uint8 index, bool flag) = 0;
//		virtual bool	getValue(uint8 index) const = 0;
//		virtual size_t	getBitCount() const = 0;
//	};
//
//	template<size_t BitCount>
//	class MaterialParameterHolder_BitFlag : public IMaterialParameterHolder_BitFlag
//	{
//		static_assert(BitCount == 8 || BitCount == 16 || BitCount == 32, "BitCount must be 8, 16, or 32.");
//		static constexpr size_t Size = BitCount / 8;
//
//	public:
//		inline virtual const void*		getReadPtr() const override { return &_bitFlags; }
//		inline virtual void*			getWritePtr() override { return &_bitFlags; }
//		inline virtual size_t			getMemorySize() const override { return Size; }
//		virtual MaterialParameterType	getType() const override;
//
//	public:
//		virtual void			setValue(uint8 index, bool flag) override;
//		virtual bool			getValue(uint8 index) const override;
//		inline virtual size_t	getBitCount() const override { return BitCount; }
//
//	protected:
//		uint8 _bitFlags[Size] = { 0 };
//	};
//
//	using MaterialParameterHolder_BitFlag8 = MaterialParameterHolder_BitFlag<8>;
//	using MaterialParameterHolder_BitFlag16 = MaterialParameterHolder_BitFlag<16>;
//	using MaterialParameterHolder_BitFlag32 = MaterialParameterHolder_BitFlag<32>;
//
//	class MaterialParamaterHolderRefencedPath_Texture : public IMaterialParameterHolder
//	{
//	public:
//		virtual MaterialParameterType	getType() const override { return MaterialParameterType::ReferencedPath_Texture; }
//		virtual const void*				getReadPtr() const override { return _path.getBuffer(); }
//		virtual void*					getWritePtr() override { return _path.getBuffer(); }
//		virtual size_t					getMemorySize() const override { return _path.getSizeBytes(); }
//
//	public:
//		inline void						setPath(const utf8* value, size_t size) { _path.write(value, size); }
//
//	protected:
//		StaticBuffer<utf8, kMaxPathLength>	_path;
//		uint32								_resourceAllocatedIndex = 0;
//	};
}

#include "MaterialParameterHolder.hpp"