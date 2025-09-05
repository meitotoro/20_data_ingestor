#include "SerialDevice.h"

SerialDevice::SerialDevice(const DeviceConfig& config):DeviceBase(config), thread_(nullptr), stop_flag_(false) {
	// 视频通常使用UDP
	io_ = std::make_unique<LinuxSerialIO>(config.name, config.ipAddress, config.port);
}

bool SerialDevice::start()
{
    return false;
}

void SerialDevice::stop()
{
}

void SerialDevice::readLoop()
{
}
