#pragma once
#include "LinuxUdpIO.h"
#include "BeidouDataGenerator.h"
class BeidouDataSender
{
public:
    BeidouDataSender(const std::string& name,const std::string& remoteIp, uint16_t remotePort,
        const std::string& userId, double baseLat, double baseLon, double baseAlt);
    void startSending(int intervalMs = 1000);
    void stop();

private:
    LinuxUdpIO io_;
    BeidouDataGenerator generator_;
    std::atomic<bool> running_{ false };

};

