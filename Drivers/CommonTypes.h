#pragma once
#include <string>
#include <vector>

// 设备类型枚举
enum class DeviceType { ETHERNET, SERIAL, UNKNOWN };
enum class DataSource { BEIDOU, SATCOM, VIDEO, UNKNOWN };

//协议类型
enum class ProtocolType {
    BEIDOU_CUSTOM,  // 北斗自定义
    SATCOM_PDXP,    // 卫通PDXP
    SATCOM_HDLC,    // 卫通HDLC
    VIDEO_RTP,      // 视频RTP
    VIDEO_UDP,       // 视频UDP
    UNKNOWN
};

// 通用解析结果
struct ParseResult {
    DataSource source;
    ProtocolType protocol;
    std::string dataType;    // 如"位置"/"弹道"
    std::vector<uint8_t> payload;
    std::vector<uint8_t> raw; // 原始数据（用于转发）
};
