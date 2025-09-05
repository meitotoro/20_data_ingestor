#include "BeidouDataGenerator.h"

BeidouDataGenerator::BeidouDataGenerator(const std::string& userId, double baseLat, double baseLon, double baseAlt) : userId_(userId), baseLat_(baseLat), baseLon_(baseLon), baseAlt_(baseAlt),
gen_(std::random_device{}()), dist_(-0.0001, 0.0001)
{

}

std::string BeidouDataGenerator::generatePositionData()
{
	// 获取当前时间
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);
	auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()) % 1000;
	// 使用线程安全的方法获取本地时间
	std::tm time_info;

	// 跨平台的安全时间获取
#if defined(_WIN32) || defined(_WIN64)
	// Windows 平台使用 localtime_s
	localtime_s(&time_info, &now_time_t);
#else
	// Linux/Unix 平台使用 localtime_r
	localtime_r(&now_time_t, &time_info);
#endif

	// 格式化时间 HHMMSS.sss
	std::ostringstream timeStream;
	timeStream << std::setfill('0')
		<< std::setw(2) << time_info.tm_hour
		<< std::setw(2) << time_info.tm_min
		<< std::setw(2) << time_info.tm_sec
		<< "." << std::setw(3) << now_ms.count();

	// 生成微小变化的经纬度
	double latVariation = dist_(gen_);
	double lonVariation = dist_(gen_);
	double altVariation = dist_(gen_) * 10; // 高程变化幅度稍大

	double currentLat = baseLat_ + latVariation;
	double currentLon = baseLon_ + lonVariation;
	double currentAlt = baseAlt_ + altVariation;

	// 转换经纬度为度分格式
	std::string latStr = convertToDmFormat(std::abs(currentLat));
	std::string lonStr = convertToDmFormat(std::abs(currentLon));

	// 确定方向
	char latDir = currentLat >= 0 ? 'N' : 'S';
	char lonDir = currentLon >= 0 ? 'E' : 'W';

	// 构建北斗协议消息
	std::ostringstream message;
	message << "$CCWAA,"                    // 信号类型: 北斗GGA
		<< "1,"                         // 信号类型: 1，固定值
		<< "1,"                         // 信号频度: 1Hz
		<< userId_ << ","               // 用户ID
		<< timeStream.str() << ","      // 时间
		<< latStr << ","                // 纬度
		<< latDir << ","                // 纬度方向
		<< lonStr << ","                // 经度
		<< lonDir << ","                // 经度方向
		<< "1,"                         // 定位质量指示器
		<< "08,"                        // 使用卫星数量
		<< "1.2,"                       // HDOP水平精度因子
		<< std::fixed << std::setprecision(2) << currentAlt << ","  // 高程值
		<< "M,"                         // 高程单位: 米
		<< "0.0,"                       // 大地水准面高度
		<< "M,"                         // 大地水准面高度单位
		<< "*";                         // 校验和开始标记

	// 计算校验和
	std::string data = message.str();
	uint8_t checksum = calculateChecksum(data);

	// 添加校验和和结束符
	std::ostringstream finalMessage;
	finalMessage << data
		<< std::hex << std::uppercase << std::setw(2) << std::setfill('0')
		<< static_cast<int>(checksum)
		<< "\r\n";  // <CR><LF>结束符

	return finalMessage.str();
}

std::string BeidouDataGenerator::convertToDmFormat(double decimalDegrees)
{
	int degrees = static_cast<int>(decimalDegrees);
	double minutes = (decimalDegrees - degrees) * 60.0;

	std::ostringstream stream;
	stream << degrees << std::fixed << std::setprecision(4)
		<< std::setw(2) << std::setfill('0') << minutes;

	return stream.str();
}

uint8_t BeidouDataGenerator::calculateChecksum(const std::string& data)
{
	// 找到$的位置和*的位置
	size_t start = data.find('$');
	if (start == std::string::npos) return 0;

	size_t end = data.find('*');
	if (end == std::string::npos || end <= start) return 0;

	// 计算从$后到*前的所有字符的异或值
	uint8_t checksum = 0;
	for (size_t i = start + 1; i < end; ++i) {
		checksum ^= static_cast<uint8_t>(data[i]);
	}

	return checksum;
}
