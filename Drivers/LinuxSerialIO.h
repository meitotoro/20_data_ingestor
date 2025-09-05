#pragma once
#include <QObject>
#include <string>
#include <qbytearray.h>
#include <qserialport.h>
#include <memory>
#include "PlatformIO.h"

class LinuxSerialIO : public QObject, PlatformIO
{
    Q_OBJECT
public:
    LinuxSerialIO(const std::string name, const std::string& ip, uint16_t port, QObject* parent = nullptr);

    bool open() override;
    void close() override;
    size_t read(const uint8_t* buf, size_t len) override;
    void sendData(const QByteArray& data) override;
    size_t write(const uint8_t* data, size_t len) override;


private:
    std::unique_ptr<QSerialPort> m_serialPort;
    QByteArray m_buffer;
    std::string m_name;
    std::string m_ip;
    uint16_t m_port;    

private slots:
    void handleError(QSerialPort::SerialPortError error);
    void handleReadyRead();
};

