#pragma once
#include "RhiEnum.h"

namespace keyh
{
	class IRhiDevice;

	class IRhiCommandQueue
	{
	public:
		IRhiCommandQueue(ECommandQueueType commandQueueType);
		virtual ~IRhiCommandQueue() = default;

	protected:
		ECommandQueueType _commandQueueType;

	public:
		virtual void executeCommandLists() = 0;
	};
}