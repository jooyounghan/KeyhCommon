#pragma once
#include "RhiEnum.h"

namespace keyh
{
	struct RhiFenceDesc
	{
		uint64 _initialValue = 0;
	};

	class IRhiFence
	{
	public:
		IRhiFence(const RhiFenceDesc& desc);
		virtual ~IRhiFence() = default;

	public:
		inline const RhiFenceDesc& getDesc() const { return _desc; }

	public:
		virtual void waitForSignal(uint64 timeoutInNanoseconds) = 0;

	protected:
		RhiFenceDesc _desc;
	};
}