#pragma once
#ifndef WINDOWSUDPIO_H
#define WINDOWSUDPIO_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>
#include <memory>
#include <string>
#include "PlatformIO.h" 

class WindowsUdpIO : public QObject, public PlatformIO
{
    Q_OBJECT

public:
    // 构造函数
    WindowsUdpIO(const std::string& name, const std::string& ip, uint16_t port, QObject* parent);

    // PlatformIO 接口实现
    bool open() override;
    void close() override;
    void sendData(const QByteArray& data) override;
    size_t read(const uint8_t* buffer, size_t len);
    size_t write(const uint8_t* data, size_t len) override;
    bool isConnected() const;

    // Windows特有功能
    void setBroadcastEnabled(bool enabled);
    void setMulticastGroup(const QString& multicastGroup);
    void setBufferSize(int sendBufferSize, int receiveBufferSize);
    void setReuseAddress(bool reuse);

    ~WindowsUdpIO();

public slots:
    // 公共槽函数
    void sendDatagram(const QByteArray& data);
    void sendDatagramTo(const QByteArray& data, const QString& host, quint16 port);

signals:
    // 信号
    void dataReceived(const QByteArray& data);
    void datagramReceived(const QNetworkDatagram& datagram);
    void errorOccurred(const QString& errorString);
    void connectionStatusChanged(bool connected);

private:
    std::unique_ptr<QUdpSocket> m_udpSocket;
    QHostAddress m_localAddress;
    QHostAddress m_remoteAddress;
    quint16 m_localPort;
    quint16 m_remotePort;
    bool m_isConnected;
    std::string m_name;
    QTimer* m_reconnectTimer;

    // 数据缓冲区
    QByteArray m_receiveBuffer;
    QList<QNetworkDatagram> m_pendingDatagrams;

    // Windows特有配置
    bool m_broadcastEnabled;
    QString m_multicastGroup;
    int m_sendBufferSize;
    int m_receiveBufferSize;
    bool m_reuseAddress;

private slots:
    // 私有槽函数
    void handleError(QAbstractSocket::SocketError error);
    void handleReadyRead();
    void handleStateChanged(QAbstractSocket::SocketState state);
    void attemptReconnect();

private:
    // 辅助函数
    bool bindSocket();
    void applySocketOptions();
    void setupConnections();
    QHostAddress resolveHostAddress(const std::string& ip);
};

#endif // WINDOWSUDPIO_H
