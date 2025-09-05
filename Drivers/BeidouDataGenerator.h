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
    BeidouDataGenerator(const std::string& userId, double baseLat, double baseLon, double baseAlt);
    // 生成北斗协议格式的位置数据
    std::string generatePositionData();

private:
    // 将十进制度转换为度分格式 (ddmm.mmmm)
    std::string convertToDmFormat(double decimalDegrees);
    // 计算NMEA校验和 (从$到*的所有字符异或)
    uint8_t calculateChecksum(const std::string& data);
    std::string userId_;
    double baseLat_, baseLon_, baseAlt_;
    std::mt19937 gen_;
    std::uniform_real_distribution<> dist_;
};

