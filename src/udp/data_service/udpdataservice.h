#ifndef UDPDATASERVICE_H
#define UDPDATASERVICE_H

#include <QObject>
#include "src/ultrasonic_sensor/sensordata_t.h"

// forward declaration
class MapItemsController;
struct SendData_t;

class UdpDataService : public QObject
{
    Q_OBJECT
public:
    static UdpDataService* getInstance(QObject *parent = nullptr);

public slots:
    void prepareDataToSend(SensorData_t &sensorData);
    void parseReceivedData(QByteArray data);

signals:
    void sendData(QByteArray data);
    void simulationFinished();
    void moveableChanged(bool moveable);

private:
    bool outOfTheMap(const SendData_t& sendData);

    static UdpDataService *m_instance;
    explicit UdpDataService(QObject *parent = nullptr);

    MapItemsController *m_itemsController;
};

#endif // UDPDATASERVICE_H
