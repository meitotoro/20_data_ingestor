#include "LinuxSerialIO.h"
#include<qdebug.h>

LinuxSerialIO::LinuxSerialIO(const std::string name, const std::string& ip, uint16_t port, QObject* parent) : m_name(name), m_ip(ip), m_port(port), QObject(parent)
{
    connect(m_serialPort.get(), &QSerialPort::readyRead, this, &LinuxSerialIO::handleReadyRead);
    connect(m_serialPort.get(), &QSerialPort::errorOccurred, this, &LinuxSerialIO::handleError);
}

bool LinuxSerialIO::open()
{
	// 如果串口已打开，先关闭
	if (m_serialPort->isOpen()) {
		m_serialPort->close();
	}

	// 设置串口参数
	m_serialPort->setPortName(QString::fromLocal8Bit(m_name.c_str()));
	m_serialPort->setBaudRate(QSerialPort::Baud115200);
	m_serialPort->setDataBits(QSerialPort::Data8);
	m_serialPort->setParity(QSerialPort::NoParity);
	m_serialPort->setStopBits(QSerialPort::OneStop);
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

	if (m_serialPort->open(QIODevice::ReadWrite)) {
		qDebug() << "Serial port opened successfully.";
		return true;
	}
	else {
		qDebug() << "Failed to open serial port:" << m_serialPort->errorString();
		m_serialPort->errorOccurred(m_serialPort->error());
		return false;
	}
}

void LinuxSerialIO::close()
{
	if (m_serialPort->isOpen()) {
		m_serialPort->close();
		qDebug() << "Serial port closed.";
	}
}

size_t LinuxSerialIO::read(const uint8_t* buf, size_t len)
{
	return len;
}

size_t LinuxSerialIO::write(const uint8_t* data, size_t len)
{
    return size_t();
}

void LinuxSerialIO::handleReadyRead()
{
	QByteArray data = m_serialPort->readAll();
	if (!data.isEmpty()) {
		qDebug() << "Received data:" << data.toHex(' ');
		// 获取指向数据的 const uint8_t 指针（只读访问）
		const uint8_t* buf = reinterpret_cast<const uint8_t*>(data.constData());

		// 获取数据长度
		size_t bufSize = static_cast<size_t>(data.size());
		read(buf, bufSize);
	}
}

void LinuxSerialIO::sendData(const QByteArray& data)
{
	if (m_serialPort->isOpen() && m_serialPort->isWritable()) {
		qint64 bytesWritten = m_serialPort->write(data);
		if (bytesWritten == -1) {
			qDebug() << "Error writing to serial port:" << m_serialPort->errorString();
		}
		else {
			qDebug() << "Data sent, bytes written:" << bytesWritten;
		}
	}
	else {
		qDebug() << "Cannot send data. Port is not open or not writable.";
	}
}

void LinuxSerialIO::handleError(QSerialPort::SerialPortError error)
{
	qDebug() << error;
}
