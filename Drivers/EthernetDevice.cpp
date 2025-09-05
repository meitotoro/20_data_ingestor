#include "EthernetDevice.h"
#include <iostream>

EthernetDevice::EthernetDevice(const DeviceConfig& config) : DeviceBase(config), thread_(nullptr),stop_flag_(false) {
	// 视频通常使用UDP
	io_ = std::make_unique<LinuxUdpIO>(config.name, config.ipAddress, config.port);
}

bool EthernetDevice::start()
{
    if (!io_->open()) {
        std::cerr << "Failed to open Ethernet device: " << config_.ipAddress << std::endl;
        return false;
    }

    is_open_ = true;
    stop_flag_ = false;

    // 启动数据读取线程
    thread_ = std::make_unique<std::thread>([this]() {
        readLoop();
        });

    std::cout << "Ethernet device started: " << config_.ipAddress <<",port:"<<config_.port<< std::endl;
    return true;
}

void EthernetDevice::stop()
{
    stop_flag_ = true;

    if (thread_ && thread_->joinable()) {
        thread_->join();
    }

    if (io_impl_) {
        io_impl_->close();
    }

    is_open_ = false;
    std::cout << "Ethernet device stopped: " << config_.ipAddress << std::endl;
}


void EthernetDevice::readLoop()
{
    std::cout << "Ethernet device reading loop started: " << config_.ipAddress << std::endl;
    while (!stop_flag_) {
        uint8_t buffer[1024];
        size_t bytes_read = io_->read(buffer, sizeof(buffer));
        if (bytes_read > 0) {
            if (data_callback_) {
                data_callback_(buffer, bytes_read);
            }
        }
        else if (bytes_read < 0) {
            // 处理读取错误
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // 短暂休眠以避免CPU占用过高
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
