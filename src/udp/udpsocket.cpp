#include "udpsocket.h"
#include <QDebug>
#include <QQmlEngine>
#include <cstring>
#include <QDataStream>

#ifdef QT_DEBUG
# define TEST_MODE
#endif

#define SANEPID_PORT 7777

UdpSocket::UdpSocket(QObject *parent) :
    QObject(parent),
    m_hostAddress(QStringLiteral("")),
    m_hostPort(0)
{
    m_socket = new QUdpSocket(this);
    m_dataService = UdpDataService::getInstance(this);

    connect(m_socket, &QUdpSocket::readyRead, this, &UdpSocket::onReadyRead);
    connect(m_dataService, &UdpDataService::sendData, this, QOverload<QByteArray>::of(&UdpSocket::send));
    connect(this, &UdpSocket::simulinkDataReceived, m_dataService, &UdpDataService::parseReceivedData);

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QString UdpSocket::hostAddress() const
{
    return m_hostAddress;
}

int UdpSocket::hostPort() const
{
    return m_hostPort;
}

void UdpSocket::initSocket()
{
    if(m_socket->isOpen()) {
        qDebug() << "Close last connection";
        m_socket->waitForDisconnected();
        m_socket->close();
    }

    m_addr = m_hostAddress.isEmpty() ?
            #ifdef TEST_MODE
                QHostAddress("192.168.100.212")
            #else
                QHostAddress::LocalHost
            #endif
              : QHostAddress(m_hostAddress);

    auto localAddr = QHostAddress(
        #ifdef TEST_MODE
            "192.168.100.1"
            #else
                "127.0.0.1"
            #endif
                );

    auto ok = m_socket->bind(localAddr, static_cast<uint16_t>(m_hostPort));

    if(ok) {
        qDebug() << "Connection bound: (" << m_addr <<", port:" << m_hostPort << ")";

        emit newData(QStringLiteral("Simulink sends to %1:%2").arg(localAddr.toString()).arg(m_hostPort));
        emit newData(QStringLiteral("Simulink receives from %1:%2").arg(m_addr.toString()).arg(SANEPID_PORT));
        emit connectionBound();

#ifdef TEST_MODE
        emit newData(QStringLiteral("You are in TEST mode: remove this macro before release!"));
#endif
    } else {
        qDebug() << "Cannot connect to " << m_addr << m_hostPort;
        emit newData(QStringLiteral("Connection refused"));
    }
}

void UdpSocket::send(QByteArray data)
{
    auto res = m_socket->writeDatagram(data, m_addr, SANEPID_PORT);
    if(res >= 0) {
        qDebug() << "datagram was sent";
        emit newData(QStringLiteral("Write: ") + QString::number(data.length()) + QStringLiteral(" bytes"));
    } else {
        qDebug() << "cannot send datagram";
        qDebug() << m_socket->errorString();
    }
}

void UdpSocket::send(QString data)
{
    send(data.toUtf8());
}

void UdpSocket::setHostAddress(QString hostAddress)
{
    if (m_hostAddress == hostAddress)
        return;

    m_hostAddress = hostAddress;
    emit hostAddressChanged(m_hostAddress);
}

void UdpSocket::setHostPort(int hostPort)
{
    if (m_hostPort == hostPort)
        return;

    m_hostPort = hostPort;
    emit hostPortChanged(m_hostPort);
}

void UdpSocket::onReadyRead()
{
    QByteArray arr;
    arr.resize(static_cast<int>(m_socket->pendingDatagramSize()));

    //sender and its port
    QHostAddress sender;
    quint16 senderPort;

    m_socket->readDatagram(arr.data(), arr.size(), &sender, &senderPort);

    qDebug() << "Datagram from:" << sender.toString()
             << "on port:"       << senderPort;

    if(arr.endsWith('\n')) {
        arr.chop(1);
    }

    emit newData(QStringLiteral("Read: ") + QString::number(arr.length()) + QStringLiteral(" bytes"));
    emit simulinkDataReceived(arr);
}
