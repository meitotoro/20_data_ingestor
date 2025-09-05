#pragma once
#include <memory>
#include <DeviceBase.h>
class DeviceFactory
{
public:
	static std::unique_ptr<DeviceBase> create(const DeviceConfig& config);
};

