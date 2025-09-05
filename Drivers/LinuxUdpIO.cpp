#include "LinuxUdpIO.h"


LinuxUdpIO::LinuxUdpIO(const std::string name, const std::string& ip, uint16_t port, QObject* parent) : m_name(name), m_remoteAddress(QString::fromLocal8Bit(ip.c_str())), m_remotePort(port), QObject(parent)
{
	// 连接信号和槽
    m_udpSocket.reset(new QUdpSocket(this));
	connect(m_udpSocket.get(), &QUdpSocket::readyRead, this, &LinuxUdpIO::handleReadyRead);
	connect(m_udpSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
		this, &LinuxUdpIO::handleError);
}

bool LinuxUdpIO::open()
{
    // 如果已经打开，先关闭
    if (m_udpSocket->state() != QAbstractSocket::UnconnectedState) {
        close();
    }

    // 尝试绑定到指定的本地地址和端口
    if (!m_udpSocket->bind(m_remoteAddress, m_remotePort)) {
        QAbstractSocket::SocketError error = m_udpSocket->error();
        qDebug()<<tr("无法绑定到端口 %1: %2")
            .arg(m_remoteAddress.toString().toLocal8Bit().constData())
            .arg(m_remotePort).toLocal8Bit();
        return false;
    }

    m_isConnected = true;
    qDebug() << "UDP客户端已启动，本地端口:" << m_udpSocket->localPort();
    return true;
}

void LinuxUdpIO::close()
{
    if (m_udpSocket->state() != QAbstractSocket::UnconnectedState) {
        m_udpSocket->close();
        m_isConnected = false;
        qDebug() << "UDP连接已关闭";
    }
}

void LinuxUdpIO::sendData(const QByteArray& data)
{
    if (m_remotePort == 0) {
       qDebug()<<QString::fromLocal8Bit("未设置远程地址和端口");
        return;
    }
    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(data.constData());

    write(dataPtr, data.size());
}

size_t LinuxUdpIO::read(const uint8_t* buf, size_t len)
{

    return size_t();
}

size_t LinuxUdpIO::write(const uint8_t* data, size_t len)
{
    if (!m_isConnected)
    {
        qDebug()<<QString::fromLocal8Bit("未连接");
        return 0;
    }
   return len;
}

LinuxUdpIO::~LinuxUdpIO()
{
	close();
}

void LinuxUdpIO::handleReadyRead()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        qint64 bytesRead = m_udpSocket->readDatagram(datagram.data(), datagram.size(),
            &senderAddress, &senderPort);

        if (bytesRead == -1) {
            qDebug()<<tr("读取数据失败: %1").arg(m_udpSocket->errorString());
            continue;
        }

        qDebug() << "接收到来自" << senderAddress.toString() << ":" << senderPort
            << "的数据，大小:" << bytesRead << "字节";
    }

}

void LinuxUdpIO::handleError(QAbstractSocket::SocketError error)
{
    qDebug()<<error;
}
