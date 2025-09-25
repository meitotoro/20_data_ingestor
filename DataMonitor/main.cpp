#include <QtCore/QCoreApplication>
#include <BeidouDataSender.h>
#include <csignal>
void sendBeidouData()
{
	try {
		// 配置参数
		std::string name = "beidouSend";
		std::string remoteIp = "127.0.0.1";  // 目标IP地址
		uint16_t remotePort = 8080;              // 目标端口

		// 配置位置数据参数
		BeidouDataGenerator::PositionConfig posConfig;
		posConfig.userId = "USER001";        // 用户ID
		posConfig.baseLat = 39.9042;         // 基准纬度 (北京)
		posConfig.baseLon = 116.4074;        // 基准经度 (北京)
		posConfig.baseAlt = 43.5;            // 基准高程 (米)

		// 配置 TCI 报文参数
		BeidouDataGenerator::TciConfig tciConfig;
		tciConfig.senderId = "001";          // 发信方 ID
		tciConfig.receiverId = "002";        // 收信方 ID
		tciConfig.frequency = "1615.125";    // 频点 (MHz)
		tciConfig.encodingType = "1";        // 编码类型
		tciConfig.commData = "Hello,北斗三代!"; // 数据通信内容

        // 创建数据发送器
        BeidouDataSender sender(name,remoteIp, remotePort, posConfig, tciConfig);

        std::cout << "Starting Beidou data sender. Press Ctrl+C to stop." << std::endl;

        // 设置信号处理，以便优雅地停止
        signal(SIGINT, [](int) {
            std::cout << "\nStopping sender..." << std::endl;
            exit(0);
            });

        // 发送单条位置数据
       // sender.sendPositionData();

        // 发送单条 TCI 报文
       // sender.sendTciMessage();

        // 或者启动周期性发送
         sender.startSending();  // 每秒发送一次位置数据
        
         // 保持主线程运行
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // 可以在这里动态修改配置或间隔
            static int counter = 0;
            if (++counter % 10 == 0) {
                // 每10秒修改一次TCI消息内容
                auto newTciConfig = sender.getGenerator().getTciConfig();
                newTciConfig.commData = "Updated message at " +
                    std::to_string(std::time(nullptr));
                sender.getGenerator().setTciConfig(newTciConfig);

                std::cout << "Updated TCI message content" << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }

}

int main(int argc, char *argv[])
{
	sendBeidouData();
    QCoreApplication app(argc, argv);

    return app.exec();
}
