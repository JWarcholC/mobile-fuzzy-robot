#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include "src/udp/data_service/udpdataservice.h"

class UdpSocket : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostAddress READ hostAddress WRITE setHostAddress NOTIFY hostAddressChanged)
    Q_PROPERTY(int hostPort READ hostPort WRITE setHostPort NOTIFY hostPortChanged)

public:
    explicit UdpSocket(QObject *parent = nullptr);

    QString hostAddress() const;
    int hostPort() const;

public slots:
    void initSocket();
    void send(QByteArray data);
    void send(QString data);

    void setHostAddress(QString hostAddress);
    void setHostPort(int hostPort);

signals:
    void hostAddressChanged(QString hostAddress);
    void hostPortChanged(int hostPort);
    void newData(QString datagram);
    void connectionBound();
    void simulinkDataReceived(QByteArray data);

private slots:
    void onReadyRead();

private:
    QUdpSocket *m_socket;
    UdpDataService *m_dataService;
    QHostAddress m_addr;
    QString m_hostAddress;
    int m_hostPort;
};

#endif // UDPSOCKET_H
