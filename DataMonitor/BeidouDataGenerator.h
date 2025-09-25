#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <iomanip>
#include <sstream>
#include <cmath>
class BeidouDataGenerator
{
public:
    // 位置数据配置
    struct PositionConfig {
        std::string userId;        // 用户ID
        double baseLat;            // 基准纬度
        double baseLon;            // 基准经度
        double baseAlt;            // 基准高程

        PositionConfig(const std::string& id = "USER001",
            double lat = 39.9042, double lon = 116.4074, double alt = 43.5)
            : userId(id), baseLat(lat), baseLon(lon), baseAlt(alt) {}
    };

    // TCI 报文配置
    struct TciConfig {
        std::string senderId;      // 发信方ID
        std::string receiverId;    // 收信方ID
        std::string frequency;     // 频点
        std::string encodingType;  // 编码类型
        std::string commData;      // 数据通信内容

        TciConfig(const std::string& sender = "001",
            const std::string& receiver = "002",
            const std::string& freq = "1615.125",
            const std::string& encoding = "1",
            const std::string& data = "Hello,北斗三代!")
            : senderId(sender), receiverId(receiver), frequency(freq),
            encodingType(encoding), commData(data) {}
    };

    // ZDA 时间数据配置
    struct ZdaConfig {
        std::string modeIndicator;      // 模式指示 (A/D/E/N)
        std::string localZoneHours;     // 本地时区小时
        std::string localZoneMinutes;   // 本地时区分钟
        std::string revisionTime;       // 定时修订时间
        std::string correctionValue;    // 修正值
        std::string longitudeIndicator; // 经度指示 (E/W)
        std::string lockStatus;         // 卫星信号锁定状态 (A/V)

        ZdaConfig(const std::string& mode = "A",
            const std::string& lzh = "08",
            const std::string& lzm = "00",
            const std::string& revTime = "",
            const std::string& corrValue = "5",
            const std::string& lonInd = "E",
            const std::string& lockStat = "A")
            : modeIndicator(mode), localZoneHours(lzh), localZoneMinutes(lzm),
            revisionTime(revTime), correctionValue(corrValue),
            longitudeIndicator(lonInd), lockStatus(lockStat) {}
    };

    BeidouDataGenerator(const PositionConfig& posConfig = PositionConfig(),
        const TciConfig& tciConfig = TciConfig(), const ZdaConfig& zdaConfig = ZdaConfig());
    // 设置位置配置
    void setPositionConfig(const PositionConfig& config);
    // 设置TCI配置
    void setTciConfig(const TciConfig& config);
    TciConfig getTciConfig() const;
    // 生成北斗协议格式的位置数据
    std::string generatePositionData();
    // 生成 TCI 报文
    std::string generateTciMessage();

private:
    // 将十进制度转换为度分格式 (ddmm.mmmm)
    std::string convertToDmFormat(double decimalDegrees);
    // 计算NMEA校验和 (从$到*的所有字符异或)
    uint8_t calculateChecksum(const std::string& data);
    PositionConfig posConfig_;
    TciConfig tciConfig_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> dist_;
};


