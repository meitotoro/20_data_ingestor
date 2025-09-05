// DriverCommon.h
#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "CommonTypes.h"
#include <PlatformIO.h>
#include <functional>

using DataCallBack=std::function<void(const uint8_t*, size_t)>;

// 设备元数据
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


// 设备基类
class DeviceBase {
public:
    DeviceBase(const DeviceConfig& config)
        : config_(config) {}

    virtual ~DeviceBase() = default;
    bool isOpen() const { return is_open_; }
    DataSource source() const { return config_.dataSource; }
    ProtocolType protocol() const { return config_.protocolType; }

    virtual bool start() = 0;
    virtual void stop() = 0;
    void setDataCallback(DataCallBack callBack) { data_callback_ = callBack; };
    const DeviceConfig& config() const { return config_; }

protected:
    DeviceConfig config_;
    bool is_open_ = false;
    std::unique_ptr<PlatformIO> io_impl_;
    DataCallBack data_callback_;
};


