#pragma once
#include "CommonTypes.h"
#include <DeviceBase.h>
#include <DataStorage.h>
#include <ProtocolForwarder.h>
#include <StorageRouter.h>

class DataProcessingServer
{
public:
	void addDevice(const DeviceConfig& config);
	void start();
	void stop();
	// 配置存储
	void configureStorage(DataSource source, std::unique_ptr<DataStorage> storage);

	// 配置转发
	void addForwardTarget(const ProtocolForwarder::ForwardTarget& target);

private:
	void handleError(const std::string& error);

private:
	std::vector<std::unique_ptr<DeviceBase>> devices_;
	StorageRouter storage_router_;
	ProtocolForwarder forwarder_;
};

