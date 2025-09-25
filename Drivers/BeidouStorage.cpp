#include <iostream>
#include "BeidouStorage.h"

const std::string BeidouStorage::DEFAULT_CONNECTION_STRING = "host=localhost dbname=20jd user=postgres password=123456 port=5432";

BeidouStorage::BeidouStorage(const std::string& connection_string):
    connection_string_(connection_string.empty()?DEFAULT_CONNECTION_STRING:connection_string)
{
}

void BeidouStorage::store(const ParseResult& result)
{
    if (result.dataType == "position") {
        // 解析位置数据并存入数据库
        storePosition(parsePosition(result.payload));
       // storeMessage(parseMessage(result.payload));
    }
    // ...其他数据类型
}

bool BeidouStorage::initializeDatabase()
{
    std::lock_guard<std::mutex> lock(connection_mutex_);
    try {
        connection_ = std::make_shared<pqxx::connection>(connection_string_);

        if (!connection_->is_open()) {
            std::cerr << "Failed to open database connection" << std::endl;
            return false;
        }

        std::cout << "Connected to database: " << connection_->dbname() << std::endl;

        // 创建表
        if (!createTables()) {
            return false;
        }

        // 初始化预处理语句
        initializePreparedStatements();

        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Database initialization failed: " << e.what() << std::endl;
        return false;
    }

    return false;
}

bool BeidouStorage::closeDatabase()
{
    return false;
}

bool BeidouStorage::createTables()
{
    try {
        pqxx::work tx(*connection_);

        // 创建位置数据表
        tx.exec(
            "CREATE TABLE IF NOT EXISTS position_data ("
            "id SERIAL PRIMARY KEY,"
            "longitude DOUBLE PRECISION NOT NULL,"
            "latitude DOUBLE PRECISION NOT NULL,"
            "altitude DOUBLE PRECISION,"
            "bd_time TIMESTAMP WITH TIME ZONE NOT NULL,"
            "created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP"
            ")"
        );

        // 创建报文数据表
        tx.exec(
            "CREATE TABLE IF NOT EXISTS message_data ("
            "id SERIAL PRIMARY KEY,"
            "message_id VARCHAR(64),"
            "source_id VARCHAR(64),"
            "destination_id VARCHAR(64),"
            "receive_time TIMESTAMP WITH TIME ZONE NOT NULL,"
            "frequency INTEGER,"
            "data_length SMALLINT,"
            "code_type SMALLINT,"
            "text VARCHAR(200),"
            "created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP"
            ")"
        );

        // 创建索引以提高查询性能
        tx.exec("CREATE INDEX IF NOT EXISTS idx_position_timestamp ON position_data(timestamp)");
        tx.exec("CREATE INDEX IF NOT EXISTS idx_message_receive_time ON message_data(receive_time)");
        tx.commit();
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Table creation failed: " << e.what() << std::endl;
        return false;
    }
}

PositionData BeidouStorage::parsePosition(const std::vector<uint8_t>& data)
{
    PositionData pos = { 0.0, 0.0, 0.0 };

    if (data.size() < 33) { // 最小长度: 1(类型) + 8×4(数据) = 33字节
        std::cerr << "Invalid position data length: " << data.size() << std::endl;
        return pos;
    }

    try {
        // 数据格式: [类型1字节][时间8字节][纬度8字节][经度8字节][海拔8字节]
        double timeValue = bytesToDouble(data, 1);
        pos.lon = bytesToDouble(data, 17); // 跳过类型1字节 + 时间8字节 + 纬度8字节
        pos.lat = bytesToDouble(data, 9);  // 跳过类型1字节 + 时间8字节
        pos.alt = bytesToDouble(data, 25); // 跳过类型1字节 + 时间8字节 + 纬度8字节 + 经度8字节
         
        pos.bd_time = std::chrono::system_clock::from_time_t(timeValue);

        // 验证数据有效性
        if (!isValidCoordinate(pos.lon) || !isValidCoordinate(pos.lat)) {
            std::cerr << "Invalid BDGGA coordinates: lon=" << pos.lon
                << ", lat=" << pos.lat << std::endl;
            return PositionData{ 0.0, 0.0, 0.0 };
        }

        // 验证时间有效性
        if (!isValidTimestamp(pos.bd_time)) {
            std::cerr << "Invalid timestamp in position data" << std::endl;
            // 可以选择使用当前时间作为默认值
            pos.receive_time = std::chrono::system_clock::now();
        }

        return pos;

    }
    catch (const std::exception& e) {
        std::cerr << "Position parsing error: " << e.what() << std::endl;
    }

    return pos;
}

MessageData BeidouStorage::parseMessage(const std::vector<uint8_t>& data)
{
    return MessageData();
}

bool BeidouStorage::isValidCoordinate(double value)
{
    // 简单的坐标范围验证
    return (value >= -180.0 && value <= 180.0);
}

bool BeidouStorage::isValidTimestamp(const std::chrono::system_clock::time_point& timestamp)
{
    auto now = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::hours>(now - timestamp);
        
        // 允许的时间范围：过去30天到未来1小时（根据应用需求调整）
        return (diff.count() <= 24 * 30 && diff.count() >= -1);
}

double BeidouStorage::bytesToDouble(const std::vector<uint8_t>& bytes, size_t offset)
{
    if (bytes.size() < offset + sizeof(double)) {
        return 0.0;
    }

    double value;
    std::memcpy(&value, bytes.data() + offset, sizeof(double));
    return value;
}

int BeidouStorage::storePosition(const PositionData& pos)
{
   pqxx::work tx(*connection_);
   try {
       pqxx::result res = tx.exec_prepared(
           "insert_position",
           pos.lon,
           pos.lat,
           pos.alt
           );
       if (!res.empty())
       {
           return res[0][0].as<int>();
       }
       return -1;
    }
   catch (const std::exception& e)
   {
       std::cerr << "Error inserting position: " << e.what() << std::endl;
       return -1;
   }

}

//int BeidouStorage::storeMessage(const MessageData& msg)
//{
//    pqxx::work tx(*connection_);
//    try {
//        pqxx::result r = tx.exec_prepared(
//            "insert_message",
//            msg.message_id,
//            msg.source_id,
//            msg.destination_id,
//            msg.receive_time,
//            msg.frequency,
//            msg.data_length,
//            msg.codeType,
//            msg.text
//        );
//
//        if (!r.empty()) {
//            return r[0][0].as<int>();
//        }
//        return -1;
//
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Failed to insert message data: " << e.what() << std::endl;
//        return -1;
//    }
//}

void BeidouStorage::initializePreparedStatements()
{
    pqxx::work tx(*connection_);
    try {
                // 创建位置数据预处理语句
        connection_->prepare("insert_position",
            "INSERT INTO position_data (longitude, latitude, altitude, timestamp) VALUES ($1, $2, $3, $4) RETURNING id"
        );

        // 创建报文数据预处理语句
        connection_->prepare("insert_message",
            "INSERT INTO message_data (message_id, source_id, destination_id, receive_time, frequency, data_length, code_type, text) VALUES ($1, $2, $3, $4, $5, $6, $7, $8) RETURNING id"
        );
    }
    catch (const std::exception& e)
    {
        std::cerr << "Prepared statement initialization failed: " << e.what() << std::endl;
    }
}
