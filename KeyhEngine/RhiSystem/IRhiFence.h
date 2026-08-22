#pragma once

namespace keyh
{
	class IRhiFence
	{
	public:
		IRhiFence() = default;
		virtual ~IRhiFence() = default;
	public:
		virtual void waitForSignal(uint64 timeoutInNanoseconds) = 0;
	};
}