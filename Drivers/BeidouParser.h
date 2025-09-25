#pragma once
#include "ProtocolParser.h"
class BeidouParser :
    public ProtocolParser
{
public:
    ParseResult parse(const uint8_t* data, size_t len) override;
    ProtocolType protocolType() const override;
private:
    // 解析BDGGA格式的位置数据
    std::vector<uint8_t> parseGGA(const std::string& message);

    //验证校验和
    bool verifyChecksum(const std::string& message);

    // 解析时间
    double parseTime(const std::string& timeStr);

    // 解析坐标格式 DDMM.MMMM + 方向 -> 十进制度数
    double parseCoordinate(const std::string& coordStr, char direction);

    // double转换为字节数组
    std::vector<uint8_t> doubleToBytes(double value);

    // 字节数组转换为double
    double bytesToDouble(const std::vector<uint8_t>& bytes, size_t offset = 0);
};

