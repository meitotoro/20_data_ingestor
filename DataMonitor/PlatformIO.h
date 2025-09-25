#pragma once
#include <stdint.h>
#include <QObject>

// 平台无关的IO接口抽象
class PlatformIO {
public:
    virtual ~PlatformIO() = default;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual size_t read(const uint8_t* buffer, size_t len) = 0;
    virtual size_t write(const uint8_t* data, size_t len) = 0;
    virtual void sendData(const QByteArray& data)=0;
};
