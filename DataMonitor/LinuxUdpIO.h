#pragma once
#include <QObject>
#include <string>
#include <qstringlist.h>
#include <QUdpSocket>
#include <memory>
#include "PlatformIO.h"


class LinuxUdpIO :  public QObject, PlatformIO
{    
    Q_OBJECT
public:
    LinuxUdpIO(const std::string name, const std::string& ip, uint16_t port, QObject *parent=nullptr);
    bool open() override;
    void close() override;
    void sendData(const QByteArray& data) override;
    size_t read(const uint8_t* buf, size_t len) override;
    size_t write(const uint8_t* data, size_t len);
    ~LinuxUdpIO();
private:   
    std::unique_ptr<QUdpSocket> m_udpSocket;
    QHostAddress m_remoteAddress;
    quint16 m_remotePort;
    bool m_isConnected;
    std::string m_name;

signals:
    void dataReceived(const QByteArray& data);

private slots:
    void handleError(QAbstractSocket::SocketError error);
    void handleReadyRead();

};

