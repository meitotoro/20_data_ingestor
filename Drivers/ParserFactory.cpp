#include "ParserFactory.h"
#include <BeidouParser.h>
#include <SatcomPDXPParser.h>
#include <stdexcept>

std::shared_ptr<ProtocolParser> ParserFactory::create(ProtocolType type)
{
    switch (type) {
    case ProtocolType::BEIDOU_CUSTOM:
        return std::make_shared<BeidouParser>();
    case ProtocolType::SATCOM_PDXP:
        return std::make_shared<SatcomPDXPParser>();
        // ...其他协议
    default:
        throw std::invalid_argument("Unsupported protocol");
    }
    
}
