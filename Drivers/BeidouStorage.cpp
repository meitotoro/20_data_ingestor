#include "BeidouStorage.h"

void BeidouStorage::store(const ParseResult& result)
{
    if (result.dataType == "position") {
        // 解析位置数据并存入数据库
        storePosition(parsePosition(result.payload));
    }
    // ...其他数据类型
}

PositionData BeidouStorage::parsePosition(const std::vector<uint8_t>& data)
{
    return PositionData();
}

void BeidouStorage::storePosition(const PositionData& pos)
{
   
}
