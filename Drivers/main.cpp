#include <QtCore/QCoreApplication>
#include <DataProcessingServer.h>
#include <BeidouStorage.h>
#include <DeviceFactory.h>
#include <LinuxUdpIO.h>
#include <iostream>
#include <BeidouDataSender.h>
#include <signal.h>
#include <csignal>
void sendBeidouData()
{
	try {
		// 配置参数
		std::string name = "beidouSend";
		std::string remoteIp = "127.0.0.1";  // 目标IP地址
		uint16_t remotePort = 80;              // 目标端口
		std::string userId = "USER001";          // 用户ID
		double baseLat = 39.9042;                // 基准纬度 (北京)
		double baseLon = 116.4074;               // 基准经度 (北京)
		double baseAlt = 43.5;                   // 基准高程 (米)

		// 创建并启动发送器
		BeidouDataSender sender(name, remoteIp, remotePort, userId, baseLat, baseLon, baseAlt);

		std::cout << "Starting Beidou data sender. Press Ctrl+C to stop." << std::endl;

		// 设置信号处理，以便优雅地停止
		::signal(SIGINT, [](int) {
			std::cout << "\nStopping sender..." << std::endl;
			// 在实际应用中，这里应该通知发送器停止
			std::exit(0);
			});

		// 开始发送数据
		sender.startSending(1000);  // 每秒发送一次

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}

}
int main(int argc, char* argv[])
{
	sendBeidouData();
	//QCoreApplication app(argc, argv);
	DataProcessingServer server;

	// 配置北斗设备1
	DeviceConfig beidouCfg;
	beidouCfg.name = "beidou1",
	beidouCfg.deviceType = DeviceType::ETHERNET,
	beidouCfg.dataSource = DataSource::BEIDOU,
	beidouCfg.ipAddress = "127.0.0.1",
	beidouCfg.port = 80,
	beidouCfg.serialPort = "",
	beidouCfg.baudRate = 9600,
	beidouCfg.dataBits = 8,
	beidouCfg.stopBits = 1,
	beidouCfg.parity = "N",
	beidouCfg.protocolType = ProtocolType::BEIDOU_CUSTOM;

	server.addDevice(beidouCfg);

	LinuxUdpIO* udp = new LinuxUdpIO("udp", "127.0.0.1", 6000);
	udp->sendData("hello world");

	// 配置存储
	server.configureStorage(DataSource::BEIDOU,
		std::make_unique<BeidouStorage>());

	// 配置转发
	ProtocolForwarder::ForwardTarget satcomTarget;
	satcomTarget.address = "10.1.2.100";
	satcomTarget.port = 6000;
	satcomTarget.protocol = ProtocolType::SATCOM_HDLC;
	server.addForwardTarget(satcomTarget);

	// 启动服务
	server.start();

	//return app.exec();
}


