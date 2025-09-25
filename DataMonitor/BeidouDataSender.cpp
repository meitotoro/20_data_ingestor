#include "BeidouDataSender.h"


BeidouDataSender::BeidouDataSender(const std::string& name, const std::string& remoteIp, uint16_t remotePort, const BeidouDataGenerator::PositionConfig& posConfig, const BeidouDataGenerator::TciConfig& tciConfig) : io_(name,remoteIp, remotePort), generator_(posConfig, tciConfig) {

	if (!io_.open()) {
		throw std::runtime_error("Failed to open UDP connection");
	}
}

bool BeidouDataSender::sendPositionData()
{
	try {
		std::string message = generator_.generatePositionData();
		io_.sendData(message.c_str());

		if (message.size() > 0) {
			std::cout << "Sent position data: " << message;
			return true;
		}
		else {
			std::cerr << "Failed to send position data" << std::endl;
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error generating or sending position data: " << e.what() << std::endl;
		return false;
	}
}

bool BeidouDataSender::sendTciMessage()
{
	try {
		std::string message = generator_.generateTciMessage();
		io_.sendData(message.c_str());

		if (message.size() > 0) {
			std::cout << "Sent TCI message: " << message;
			return true;
		}
		else {
			std::cerr << "Failed to send TCI message" << std::endl;
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error generating or sending TCI message: " << e.what() << std::endl;
		return false;
	}
}

void BeidouDataSender::setPositionInterval(int intervalMs)
{
	positionIntervalMs_ = intervalMs;
}

void BeidouDataSender::setTciInterval(int intervalMs)
{
	tciIntervalMs_ = intervalMs;
}

void BeidouDataSender::startSending()
{
	// 停止之前的发送线程
	stop();

	// 重置运行标志
	running_ = true;

	// 启动位置数据发送线程
	positionThread_ = std::thread([this]() {
		while (running_) {
			sendPositionData();
			std::this_thread::sleep_for(std::chrono::milliseconds(positionIntervalMs_));
		}
		});

	// 启动TCI报文发送线程
	tciThread_ = std::thread([this]() {
		while (running_) {
			sendTciMessage();
			std::this_thread::sleep_for(std::chrono::milliseconds(tciIntervalMs_));
		}
		});

	std::cout << "Started sending both position data and TCI messages" << std::endl;
	std::cout << "Position interval: " << positionIntervalMs_ << "ms" << std::endl;
	std::cout << "TCI interval: " << tciIntervalMs_ << "ms" << std::endl;
}

void BeidouDataSender::stop()
{
	running_ = false;

	if (positionThread_.joinable()) {
		positionThread_.join();
	}

	if (tciThread_.joinable()) {
		tciThread_.join();
	}

	std::cout << "Stopped all sending threads" << std::endl;
}

BeidouDataGenerator& BeidouDataSender::getGenerator()
{
	// TODO: 在此处插入 return 语句
	return generator_;
}
