#include <QtCore/QCoreApplication>
#include <DataProcessingServer.h>
#include <BeidouStorage.h>
#include <DeviceFactory.h>
#include <LinuxUdpIO.h>
#include <iostream>
#include <BeidouDataSender.h>
#include <signal.h>
#include <csignal>

int main(int argc, char* argv[])
{
	//QCoreApplication app(argc, argv);
	DataProcessingServer server;

	// 配置北斗设备1
	DeviceConfig beidouCfg;
	beidouCfg.name = "beidou1",
	beidouCfg.deviceType = DeviceType::ETHERNET,
	beidouCfg.dataSource = DataSource::BEIDOU,
	beidouCfg.ipAddress = "127.0.0.1",
	beidouCfg.port = 8080,
	beidouCfg.serialPort = "",
	beidouCfg.baudRate = 0,
	beidouCfg.dataBits = 8,
	beidouCfg.stopBits = 1,
	beidouCfg.parity = "N",
	beidouCfg.protocolType = ProtocolType::BEIDOU_CUSTOM;

	server.addDevice(beidouCfg);

	//LinuxUdpIO* udp = new LinuxUdpIO("udp", "127.0.0.1", 6000);
	//udp->sendData("hello world");

	// 配置存储
	server.configureStorage(DataSource::BEIDOU,
		std::make_unique<BeidouStorage>());

	// 配置转发
	/*ProtocolForwarder::ForwardTarget satcomTarget;
	satcomTarget.address = "10.1.2.100";
	satcomTarget.port = 6000;
	satcomTarget.protocol = ProtocolType::SATCOM_HDLC;
	server.addForwardTarget(satcomTarget);*/

	// 启动服务
	server.start();

	//return app.exec();
}


