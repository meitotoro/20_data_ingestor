#pragma once
#include "LinuxUdpIO.h"
#include "BeidouDataGenerator.h"
class BeidouDataSender
{
public:
    BeidouDataSender(const std::string& name,const std::string& remoteIp, uint16_t remotePort,
        const BeidouDataGenerator::PositionConfig& posConfig = BeidouDataGenerator::PositionConfig(),
        const BeidouDataGenerator::TciConfig& tciConfig = BeidouDataGenerator::TciConfig());
    bool sendPositionData();
    bool sendTciMessage();
    void setPositionInterval(int intervalMs = 1000);
    void setTciInterval(int intervalMs = 5000);
    void startSending();
    void stop();
    BeidouDataGenerator& getGenerator();

private:
    LinuxUdpIO io_;
    BeidouDataGenerator generator_;
    std::atomic<bool> running_{ false };
    std::thread positionThread_;
    std::thread tciThread_;
    int positionIntervalMs_ = 1000; // 默认1秒
    int tciIntervalMs_ = 5000;      // 默认5秒
};

