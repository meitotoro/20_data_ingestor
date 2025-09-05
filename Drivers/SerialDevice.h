#pragma once
#include "DeviceBase.h"
#include <LinuxSerialIO.h>
#include <thread>
class SerialDevice : public DeviceBase
{
public:
    SerialDevice(const DeviceConfig& config);

    // 通过 DeviceBase 继承
    bool start() override;
    void stop() override;
private:
    void readLoop();

private:
    std::unique_ptr<LinuxSerialIO> io_;
    std::unique_ptr<std::thread> thread_;
    bool stop_flag_;

};

