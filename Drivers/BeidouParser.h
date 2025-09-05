#pragma once
#include "ProtocolParser.h"
class BeidouParser :
    public ProtocolParser
{
    ParseResult parse(const uint8_t* data, size_t len) override;
    ProtocolType protocolType() const override;
};

