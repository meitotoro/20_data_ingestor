#pragma once
#include <stdint.h>
#include "CommonTypes.h"
#include <memory>

// 协议解析接口
class ProtocolParser {
public:
    virtual ~ProtocolParser() = default;
    virtual ParseResult parse(const uint8_t* data, size_t len) = 0;
    virtual ProtocolType protocolType() const = 0;
};


