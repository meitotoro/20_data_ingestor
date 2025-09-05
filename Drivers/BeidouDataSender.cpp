#include "BeidouDataSender.h"

BeidouDataSender::BeidouDataSender(const std::string& name, const std::string& remoteIp, uint16_t remotePort, const std::string& userId, double baseLat, double baseLon, double baseAlt) : io_(name, remoteIp, remotePort), generator_(userId, baseLat, baseLon, baseAlt)
{
	if (!io_.open()) {
		throw std::runtime_error("Failed to open UDP connection");
	}
}

void BeidouDataSender::startSending(int intervalMs)
{
	running_ = true;

	while (running_) {
		try {
			// 生成北斗协议数据
			std::string data = generator_.generatePositionData();

			// 转换为字节数组并发送
			std::vector<uint8_t> dataBytes(data.begin(), data.end());
			size_t sent = io_.write(dataBytes.data(), dataBytes.size());

			if (sent > 0) {
				std::cout << "Sent " << sent << " bytes: " << data;
			}
			else {
				std::cerr << "Failed to send data" << std::endl;
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error generating or sending data: " << e.what() << std::endl;
		}

		// 等待指定的时间间隔
		std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
	}
}

void BeidouDataSender::stop()
{
	running_ = false;
}
