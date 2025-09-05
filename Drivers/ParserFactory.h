#pragma once
#include <memory>
#include <ProtocolParser.h>

class ParserFactory
{
public:
	static std::shared_ptr<ProtocolParser> create(ProtocolType type);
};

