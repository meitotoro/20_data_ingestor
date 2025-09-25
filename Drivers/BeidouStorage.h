#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>
#include "pqxx/pqxx"
#include "DataStorage.h"

//位置信息
struct PositionData {
    double lon;
    double lat;
    double alt;
    std::chrono::system_clock::time_point bd_time; // 北斗时间
    std::chrono::system_clock::time_point receive_time; // 北斗时间

};

// 报文信息
struct MessageData {
    std::string message_id;      // 报文ID
    std::string source_id;       // 信源ID
    std::string destination_id;  // 信宿ID
    std::chrono::system_clock::time_point receive_time; // 接收时间
    int frequency;               //频点
    uint32_t data_length;        // 数据长度
    int codeType;                // 编码类别
    std::string text;            // 数据通信
};

class BeidouStorage :
	public DataStorage
{
public:
    BeidouStorage(const std::string& connection_string = "");
    void store(const ParseResult& result)override;
	//void store(const ParseResult& result) override;
    bool initializeDatabase() override;
    bool closeDatabase() override;

private:
    bool createTables();

	PositionData parsePosition(const std::vector<uint8_t>& data);
    MessageData parseMessage(const std::vector<uint8_t>& data);
    bool isValidCoordinate(double value);
    bool isValidTimestamp(const std::chrono::system_clock::time_point& timestamp);
    double bytesToDouble(const std::vector<uint8_t>& bytes, size_t offset);
	int storePosition(const PositionData& pos);
   // int storeMessage(const MessageData& msg);
    void initializePreparedStatements();

    std::shared_ptr<pqxx::connection> connection_;
    std::string connection_string_;
    std::mutex connection_mutex_;

    static const std::string DEFAULT_CONNECTION_STRING;


};

