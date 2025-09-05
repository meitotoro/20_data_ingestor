#include "SatcomPDXPParser.h"
#include <stdexcept>

ParseResult SatcomPDXPParser::parse(const uint8_t* data, size_t len)
{
    // 验证头部：FEH开头，固定32字节头
    if (len < 32 || data[0] != 0xFE)
        throw std::invalid_argument("Invalid PDXP header");

    // 解析头部字段
    uint16_t dataType = (data[4] << 8) | data[5];
    uint32_t dataLength = (data[28] << 24) | (data[29] << 16) | (data[30] << 8) | data[31];

    // 验证数据长度
    if (len != 32 + dataLength)
        throw std::invalid_argument("PDXP length mismatch");

    // 构建结果
    ParseResult result;
    result.source = DataSource::SATCOM;
    result.protocol = ProtocolType::SATCOM_PDXP;
   // result.dataType = getDataTypeString(dataType);
    result.payload.assign(data + 32, data + 32 + dataLength);
    result.raw.assign(data, data + len);

    return result;
}

ProtocolType SatcomPDXPParser::protocolType() const
{
    return ProtocolType();
}
