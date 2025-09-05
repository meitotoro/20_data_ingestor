#pragma once
#include "ProtocolParser.h"

class SatcomPDXPParser:public ProtocolParser
{
	ParseResult parse(const uint8_t* data, size_t len) override;
	ProtocolType protocolType() const override;
};

