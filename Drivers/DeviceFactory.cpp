#include "DeviceFactory.h"
#include <stdexcept>
#include <EthernetDevice.h>
#include <SerialDevice.h>

std::unique_ptr<DeviceBase> DeviceFactory::create(const DeviceConfig& config)
{
	if (config.deviceType == DeviceType::ETHERNET)
	{
        return std::make_unique<EthernetDevice>(config);		
	}
	else if (config.deviceType == DeviceType::SERIAL)
	{
        return std::make_unique<SerialDevice>(config);
	}
	else
	{
        throw std::runtime_error("Unknown device type");
	}
}
