#include "BeidouParser.h"
#include <stdexcept>

ParseResult BeidouParser::parse(const uint8_t* data, size_t len)
{
    ParseResult result;
    result.source = DataSource::BEIDOU;
    result.protocol = ProtocolType::BEIDOU_CUSTOM;

    // 验证格式：$开头，*结束
    if (len < 4 || data[0] != '$' || data[len - 3] != '*')
        throw std::invalid_argument("Invalid Beidou format");

    // 提取数据类型 (示例：$GPGGA -> 位置)
    const char* p = reinterpret_cast<const char*>(data);
    std::string header(p + 1, std::find(p + 1, p + 6, ','));

    // 解析逻辑
    if (header == "GPGGA") {
        result.dataType = "position";
        // 解析经纬度、时间等
    }
    // ...其他数据类型

    result.raw.assign(data, data + len);
    return result;
 }

ProtocolType BeidouParser::protocolType() const
{
    return ProtocolType::BEIDOU_CUSTOM;
}
