#pragma once
#include "Ptr.h"
namespace keyh
{
	class IRhiDevice;

	class IRhiInstance
	{
	public:
		IRhiInstance() = default;
		virtual ~IRhiInstance() = default;

	public:
		static Ptr<IRhiInstance> create();

	public:
		virtual bool initialize() = 0;

	public:
		inline OwnerVector<IRhiDevice>& getDevices() { return _devices; }

	protected:
		OwnerVector<IRhiDevice>	_devices;
	};
}

