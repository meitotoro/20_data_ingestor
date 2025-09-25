#include "WindowsUdpIO.h"
#include <QNetworkInterface>
#include <QDebug>
#include <chrono>

WindowsUdpIO::WindowsUdpIO(const std::string& name, const std::string& ip, uint16_t port, QObject* parent)
    : QObject(parent)
    , m_remoteAddress(resolveHostAddress(ip))
    , m_remotePort(port)
    , m_localPort(0) // 0表示自动分配端口
    , m_isConnected(false)
    , m_name(name)
    , m_broadcastEnabled(false)
    , m_sendBufferSize(8192)  // 8KB默认发送缓冲区
    , m_receiveBufferSize(65536) // 64KB默认接收缓冲区
    , m_reuseAddress(false)
{
    m_udpSocket = std::make_unique<QUdpSocket>(this);
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);

    setupConnections();

    qDebug() << "WindowsUdpIO created:" << QString::fromStdString(name)
        << "Remote:" << m_remoteAddress.toString() << ":" << m_remotePort;
}

WindowsUdpIO::~WindowsUdpIO()
{
    close();
    qDebug() << "WindowsUdpIO destroyed:" << QString::fromStdString(m_name);
}

bool WindowsUdpIO::open()
{
    if (m_isConnected) {
        qDebug() << "UDP socket already connected";
        return true;
    }

    // 关闭可能的现有连接
    if (m_udpSocket->state() != QAbstractSocket::UnconnectedState) {
        m_udpSocket->close();
    }

    // 应用socket选项
    applySocketOptions();

    // 绑定socket
    if (!bindSocket()) {
        emit errorOccurred("Failed to bind UDP socket");
        return false;
    }

    // 加入多播组（如果配置了）
    if (!m_multicastGroup.isEmpty()) {
        QHostAddress multicastAddress(m_multicastGroup);
        if (multicastAddress.protocol() == QAbstractSocket::IPv4Protocol) {
            if (!m_udpSocket->joinMulticastGroup(multicastAddress)) {
                qWarning() << "Failed to join multicast group:" << m_multicastGroup;
            }
            else {
                qDebug() << "Joined multicast group:" << m_multicastGroup;
            }
        }
    }

    m_isConnected = true;
    emit connectionStatusChanged(true);

    qDebug() << "UDP socket opened successfully. Local port:" << m_udpSocket->localPort();
    return true;
}

void WindowsUdpIO::close()
{
    if (!m_isConnected) {
        return;
    }

    // 离开多播组
    if (!m_multicastGroup.isEmpty()) {
        QHostAddress multicastAddress(m_multicastGroup);
        m_udpSocket->leaveMulticastGroup(multicastAddress);
    }

    m_udpSocket->close();
    m_isConnected = false;
    m_reconnectTimer->stop();

    emit connectionStatusChanged(false);
    qDebug() << "UDP socket closed";
}

void WindowsUdpIO::sendData(const QByteArray& data)
{
    sendDatagram(data);
}

void WindowsUdpIO::sendDatagram(const QByteArray& data)
{
    if (!m_isConnected || !m_udpSocket) {
        qWarning() << "Cannot send data: UDP socket not connected";
        return;
    }

    qint64 bytesSent = m_udpSocket->writeDatagram(data, m_remoteAddress, m_remotePort);
    if (bytesSent == -1) {
        qWarning() << "Failed to send UDP datagram:" << m_udpSocket->errorString();
        emit errorOccurred("Send failed: " + m_udpSocket->errorString());
    }
    else if (bytesSent != data.size()) {
        qWarning() << "Partial send:" << bytesSent << "of" << data.size() << "bytes";
    }
    else {
        qDebug() << "Sent" << bytesSent << "bytes to" << m_remoteAddress.toString() << ":" << m_remotePort;
    }
}

void WindowsUdpIO::sendDatagramTo(const QByteArray& data, const QString& host, quint16 port)
{
    if (!m_isConnected || !m_udpSocket) {
        qWarning() << "Cannot send data: UDP socket not connected";
        return;
    }

    QHostAddress targetAddress(host);
    if (targetAddress.isNull()) {
        // 尝试解析主机名
        targetAddress = QHostAddress(host);
        if (targetAddress.isNull()) {
            qWarning() << "Invalid target address:" << host;
            return;
        }
    }

    qint64 bytesSent = m_udpSocket->writeDatagram(data, targetAddress, port);
    if (bytesSent == -1) {
        qWarning() << "Failed to send UDP datagram to" << host << ":" << port
            << "Error:" << m_udpSocket->errorString();
    }
    else {
        qDebug() << "Sent" << bytesSent << "bytes to" << host << ":" << port;
    }
}

size_t WindowsUdpIO::read(const uint8_t* buf, size_t len)
{
    if (!m_isConnected || m_pendingDatagrams.isEmpty()) {
        return 0;
    }

    QNetworkDatagram datagram = m_pendingDatagrams.takeFirst();
    QByteArray data = datagram.data();
    size_t bytesToCopy = std::min(static_cast<size_t>(data.size()), len);

    memcpy((void*)buf, data.constData(), bytesToCopy);
    return bytesToCopy;
}

size_t WindowsUdpIO::write(const uint8_t* data, size_t len)
{
    QByteArray byteArray(reinterpret_cast<const char*>(data), static_cast<int>(len));
    sendDatagram(byteArray);
    return len; // 假设总是成功发送
}

bool WindowsUdpIO::isConnected() const
{
    return m_isConnected && m_udpSocket &&
        m_udpSocket->state() == QAbstractSocket::BoundState;
}

void WindowsUdpIO::setBroadcastEnabled(bool enabled)
{
    m_broadcastEnabled = enabled;
    if (m_udpSocket) {
        m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, enabled ? 1 : 0);
    }
}

void WindowsUdpIO::setMulticastGroup(const QString& multicastGroup)
{
    m_multicastGroup = multicastGroup;
    // 实际的多播组加入/离开在open/close中处理
}

void WindowsUdpIO::setBufferSize(int sendBufferSize, int receiveBufferSize)
{
    m_sendBufferSize = sendBufferSize;
    m_receiveBufferSize = receiveBufferSize;

    if (m_udpSocket && m_isConnected) {
        m_udpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, m_sendBufferSize);
        m_udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, m_receiveBufferSize);
    }
}

void WindowsUdpIO::setReuseAddress(bool reuse)
{
    m_reuseAddress = reuse;
    if (m_udpSocket) {
        m_udpSocket->setSocketOption(static_cast<QAbstractSocket::SocketOption>(QAbstractSocket::ReuseAddressHint), reuse);
    }
}

// ============ 私有方法实现 ============

void WindowsUdpIO::setupConnections()
{
    connect(m_udpSocket.get(), &QUdpSocket::errorOccurred,
        this, &WindowsUdpIO::handleError);
    connect(m_udpSocket.get(), &QUdpSocket::readyRead,
        this, &WindowsUdpIO::handleReadyRead);
    connect(m_udpSocket.get(), &QUdpSocket::stateChanged,
        this, &WindowsUdpIO::handleStateChanged);
    connect(m_reconnectTimer, &QTimer::timeout,
        this, &WindowsUdpIO::attemptReconnect);
}

bool WindowsUdpIO::bindSocket()
{
    // 选择绑定地址
    QHostAddress bindAddress = QHostAddress::AnyIPv4;

    // 尝试绑定
    if (!m_udpSocket->bind(bindAddress, m_localPort,
        m_reuseAddress ? QUdpSocket::ReuseAddressHint : QUdpSocket::DefaultForPlatform)) {
        qWarning() << "Bind failed:" << m_udpSocket->errorString();

        // 如果指定端口被占用，尝试自动分配端口
        if (m_localPort != 0 && m_udpSocket->error() == QAbstractSocket::AddressInUseError) {
            qDebug() << "Port" << m_localPort << "in use, trying auto-assigned port";
            m_localPort = 0; // 让系统自动分配端口
            if (!m_udpSocket->bind(bindAddress, m_localPort)) {
                qWarning() << "Auto-assign bind also failed:" << m_udpSocket->errorString();
                return false;
            }
        }
        else {
            return false;
        }
    }

    return true;
}

void WindowsUdpIO::applySocketOptions()
{
    // 设置广播选项
    m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,
        m_broadcastEnabled ? 1 : 0);

    // 设置缓冲区大小
    m_udpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, m_sendBufferSize);
    m_udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, m_receiveBufferSize);

    // 设置地址重用
    m_udpSocket->setSocketOption(static_cast<QAbstractSocket::SocketOption>(QAbstractSocket::ReuseAddressHint), m_reuseAddress);
}

QHostAddress WindowsUdpIO::resolveHostAddress(const std::string& ip)
{
    if (ip == "255.255.255.255") {
        return QHostAddress::Broadcast;
    }
    else if (ip == "0.0.0.0") {
        return QHostAddress::AnyIPv4;
    }
    else {
        QHostAddress address(QString::fromStdString(ip));
        if (address.isNull()) {
            qWarning() << "Invalid IP address:" << QString::fromStdString(ip);
            return QHostAddress::LocalHost;
        }
        return address;
    }
}

// ============ 私有槽函数实现 ============

void WindowsUdpIO::handleError(QAbstractSocket::SocketError error)
{
    QString errorMsg = QString("UDP Socket Error [%1]: %2")
        .arg(error)
        .arg(m_udpSocket->errorString());

    qWarning() << errorMsg;
    emit errorOccurred(errorMsg);

    // 对于连接错误，尝试重连
    if (error == QAbstractSocket::NetworkError ||
        error == QAbstractSocket::ConnectionRefusedError) {
        m_reconnectTimer->start(5000); // 5秒后重连
    }
}

void WindowsUdpIO::handleReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        if (!datagram.isValid()) {
            continue;
        }

        QByteArray data = datagram.data();

        // 存储数据报供read()方法使用
        m_pendingDatagrams.append(datagram);

        // 发出信号通知数据到达
        emit dataReceived(data);
        emit datagramReceived(datagram);

        qDebug() << "Received" << data.size() << "bytes from"
            << datagram.senderAddress().toString() << ":" << datagram.senderPort();
    }
}

void WindowsUdpIO::handleStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "UDP socket state changed to:" << state;

    bool wasConnected = m_isConnected;
    m_isConnected = (state == QAbstractSocket::BoundState);

    if (wasConnected != m_isConnected) {
        emit connectionStatusChanged(m_isConnected);
    }
}

void WindowsUdpIO::attemptReconnect()
{
    if (!m_isConnected) {
        qDebug() << "Attempting to reconnect UDP socket...";
        open();
    }
}