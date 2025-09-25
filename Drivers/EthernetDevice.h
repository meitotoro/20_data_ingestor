#pragma once
#include "DeviceBase.h"
#include "DeviceBase.h"
#include "LinuxSerialIO.h"
#include "LinuxUdpIO.h"
#include "WindowsUdpIO.h"
#include <thread>
#include <atomic>
#include <functional>

class EthernetDevice : public DeviceBase
{
public:
    EthernetDevice(const DeviceConfig& config);


    // 通过 DeviceBase 继承
    bool start() override;

    void stop() override;

    
private:
    void readLoop();

private:
    std::unique_ptr<WindowsUdpIO> io_;
    std::unique_ptr<std::thread> thread_;
    bool stop_flag_;

};

