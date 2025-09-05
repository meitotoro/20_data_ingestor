#pragma once
#include <string>
#include <cstdint>
#include "CommonTypes.h"

// 设备配置结构体 - 包含所有公共元数据
struct DeviceConfig {
    std::string name;           // 设备名称
    DeviceType deviceType;      // 设备类型（网口、串口）
    DataSource dataSource; // 数据类型（北斗、卫通、视频）

    // 网络连接相关配置
    std::string ipAddress;      // IP地址
    uint16_t port;              // 端口号

    // 串口连接相关配置
    std::string serialPort;     // 串口设备路径（如 "/dev/ttyUSB0"）
    uint32_t baudRate;          // 波特率
    uint8_t dataBits;           // 数据位
    uint8_t stopBits;           // 停止位
    std::string parity;         // 校验位

    ProtocolType protocolType;  // 协议类型

    // 构造函数，提供默认值
    DeviceConfig() :
        deviceType(DeviceType::UNKNOWN),
        dataSource(DataSource::UNKNOWN),
        port(0),
        baudRate(9600),
        dataBits(8),
        stopBits(1),
        parity("N"),
        protocolType(ProtocolType::UNKNOWN)
    {}
};

