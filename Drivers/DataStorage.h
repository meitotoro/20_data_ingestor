#pragma once
#include <CommonTypes.h>

// 存储接口
class DataStorage {
public:
    virtual ~DataStorage() = default;
    virtual void store(const ParseResult& result) = 0;

    //数据库连接管理
    virtual bool initializeDatabase() = 0;
    virtual bool closeDatabase() = 0;

};

