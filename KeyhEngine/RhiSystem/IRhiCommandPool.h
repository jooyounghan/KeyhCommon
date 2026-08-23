#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiCommandList;
	class IRhiDevice;

	class IRhiCommandPool
	{
	public:
		IRhiCommandPool(IRhiDevice* device, ECommandQueueType queueType);
		virtual ~IRhiCommandPool() = default;

	public:
		virtual  Ptr<IRhiCommandList> allocateCommandList() = 0;
		virtual void reset() = 0;

	public:
		inline IRhiDevice* getDevice() const { return _device; }
		inline ECommandQueueType getQueueType() const { return _queueType; }

	protected:
		IRhiDevice*			_device;
		ECommandQueueType	_queueType;
	};
}
