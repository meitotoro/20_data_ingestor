#include "BeidouParser.h"
#include <stdexcept>
#include <sstream>
#include <iostream>


ParseResult BeidouParser::parse(const uint8_t* data, size_t len)
{
    ParseResult result;
    result.source = DataSource::BEIDOU;
    result.protocol = ProtocolType::BEIDOU_CUSTOM;

    std::string message(reinterpret_cast<const char*>(data), len);

    // 验证格式：$开头，*结束
    if (len < 4 || data[0] != '$' || data[len - 3] != '*')
        throw std::invalid_argument("Invalid Beidou format");

    // 提取数据类型 (示例：$GPGGA -> 位置)
    const char* p = reinterpret_cast<const char*>(data);
    std::string header(p + 1, std::find(p + 1, p + 6, ','));

    // 解析逻辑
    if (header == "BDGGA") {
        result.dataType = "position";
        // 解析经纬度、时间等
        result.payload = parseGGA(message);
    }
    // ...其他数据类型

    result.raw.assign(data, data + len);
    return result;
 }

ProtocolType BeidouParser::protocolType() const
{
    return ProtocolType::BEIDOU_CUSTOM;
}

std::vector<uint8_t> BeidouParser::parseGGA(const std::string& message)
{
    std::vector<uint8_t> payload;
    try {
        // 分割消息字段
        std::vector<std::string> fields;
        std::stringstream ss(message);
        std::string field;

        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // 检查字段数量（BDGGA通常有15个字段）
        if (fields.size() < 15) {
            std::cerr << "Invalid BDGGA message format" << std::endl;
            return payload;
        }

        // 验证校验和
        if (!verifyChecksum(message)) {
            std::cerr << "BDGGA checksum verification failed" << std::endl;
            return payload;
        }

        // 提取关键信息并序列化到payload
        // 格式: 数据类型标记(1字节) + 时间(8字节) + 纬度(8字节) + 经度(8字节) + 海拔(8字节)
        payload.push_back(0x01); // BDGGA数据类型标记

        // 解析时间 (字段2: 时间HHMMSS.SSS)
        if (!fields[1].empty()) {
            double time = parseTime(fields[1]);
            auto timeBytes = doubleToBytes(time);
            payload.insert(payload.end(), timeBytes.begin(), timeBytes.end());
        }
        else {
            // 填充默认值
            std::vector<uint8_t> defaultTime(8, 0);
            payload.insert(payload.end(), defaultTime.begin(), defaultTime.end());
        }

        // 解析纬度 (字段3: 纬度DDMM.MMMM, 字段4: 方向N/S)
        if (!fields[2].empty() && !fields[3].empty()) {
            double latitude = parseCoordinate(fields[2], fields[3][0]);
            auto latBytes = doubleToBytes(latitude);
            payload.insert(payload.end(), latBytes.begin(), latBytes.end());
        }
        else {
            std::vector<uint8_t> defaultLat(8, 0);
            payload.insert(payload.end(), defaultLat.begin(), defaultLat.end());
        }

        // 解析经度 (字段5: 经度DDDMM.MMMM, 字段6: 方向E/W)
        if (!fields[4].empty() && !fields[5].empty()) {
            double longitude = parseCoordinate(fields[4], fields[5][0]);
            auto lonBytes = doubleToBytes(longitude);
            payload.insert(payload.end(), lonBytes.begin(), lonBytes.end());
        }
        else {
            std::vector<uint8_t> defaultLon(8, 0);
            payload.insert(payload.end(), defaultLon.begin(), defaultLon.end());
        }

        // 解析海拔 (字段10: 海拔高度)
        if (!fields[9].empty()) {
            double altitude = std::stod(fields[9]);
            auto altBytes = doubleToBytes(altitude);
            payload.insert(payload.end(), altBytes.begin(), altBytes.end());
        }
        else {
            std::vector<uint8_t> defaultAlt(8, 0);
            payload.insert(payload.end(), defaultAlt.begin(), defaultAlt.end());
        }

    }
    catch (const std::exception& e) {
        std::cerr << "BDGGA parsing error: " << e.what() << std::endl;
    }

    return payload;
}

bool BeidouParser::verifyChecksum(const std::string& message)
{
    size_t asteriskPos = message.find('*');
    if (asteriskPos == std::string::npos) {
        return false;
    }

    std::string data = message.substr(1, asteriskPos - 1); // 去掉$和*
    std::string checksumStr = message.substr(asteriskPos + 1, 2);

    uint8_t calculatedChecksum = 0;
    for (char c : data) {
        calculatedChecksum ^= c;
    }

    uint8_t receivedChecksum = std::stoi(checksumStr, nullptr, 16);
    return calculatedChecksum == receivedChecksum;
}

double BeidouParser::parseTime(const std::string& timeStr)
{
    if (timeStr.length() < 6) return 0.0;

    int hours = std::stoi(timeStr.substr(0, 2));
    int minutes = std::stoi(timeStr.substr(2, 2));
    double seconds = std::stod(timeStr.substr(4));

    return hours * 3600.0 + minutes * 60.0 + seconds;
}

double BeidouParser::parseCoordinate(const std::string& coordStr, char direction)
{
    if (coordStr.empty()) return 0.0;

    size_t dotPos = coordStr.find('.');
    if (dotPos == std::string::npos || dotPos < 2) {
        return 0.0;
    }

    int degrees = std::stoi(coordStr.substr(0, dotPos - 2));
    double minutes = std::stod(coordStr.substr(dotPos - 2));

    double decimalDegrees = degrees + minutes / 60.0;

    // 根据方向调整正负
    if (direction == 'S' || direction == 'W') {
        decimalDegrees = -decimalDegrees;
    }

    return decimalDegrees;
 }

std::vector<uint8_t> BeidouParser::doubleToBytes(double value)
{
    std::vector<uint8_t> bytes(sizeof(double));
    std::memcpy(bytes.data(), &value, sizeof(double));
    return bytes;
}

double BeidouParser::bytesToDouble(const std::vector<uint8_t>& bytes, size_t offset)
{
    if (bytes.size() < offset + sizeof(double)) {
        return 0.0;
    }

    double value;
    std::memcpy(&value, bytes.data() + offset, sizeof(double));
    return value;
}
