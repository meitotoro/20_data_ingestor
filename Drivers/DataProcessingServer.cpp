#include "DataProcessingServer.h"
#include "DeviceFactory.h"
#include <ParserFactory.h>
#include <stdexcept>
#include <iostream>

void DataProcessingServer::addDevice(const DeviceConfig& config)
{
	try {
		auto device = DeviceFactory::create(config);
		auto parser = ParserFactory::create(config.protocolType);
		if (!parser) {
			throw std::runtime_error("Invalid protocol type");
		}
		// 关键：设置数据回调，将设备与解析器绑定
		device->setDataCallback([this, parser = std::move(parser)](const uint8_t* data, size_t len)
			{
				// 获取对应的协议解析器
				try {
					// 解析数据
					auto result = parser->parse(data, len);

					// 存储数据
					storage_router_.store(result);

					// 转发数据
					forwarder_.forward(result);

				}
				catch (const std::exception& e) {
					handleError(std::string("Parse error: ") + e.what());
				}
			});

		devices_.push_back(std::move(device));
	}
	catch (const std::exception& e) {
		handleError(std::string("Device creation error: ") + e.what());
	}

}

void DataProcessingServer::start()
{
	for (auto& device : devices_) {
		device->start();
	}
	std::cout << "All devices started" << std::endl;
}

void DataProcessingServer::stop()
{
	for (auto& device : devices_) {
		device->stop();
	}
	std::cout << "All devices stopped" << std::endl;
}


void DataProcessingServer::configureStorage(DataSource source, std::unique_ptr<DataStorage> storage)
{
	storage_router_.addStorage(source, std::move(storage));
}

void DataProcessingServer::addForwardTarget(const ProtocolForwarder::ForwardTarget& target)
{
	forwarder_.addTarget(target);
}

void DataProcessingServer::handleError(const std::string& error) {
	std::cerr << "Error: " << error << std::endl;
	// 这里可以添加更复杂的错误处理逻辑
}
