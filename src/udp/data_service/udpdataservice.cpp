#include "udpdataservice.h"
#include <QDataStream>
#include <QDebug>

#include "src/udp/data/receivedata_t.h"
#include "src/udp/data/senddata_t.h"
#include "src/map_item/mapitemscontroller.h"

UdpDataService* UdpDataService::m_instance = nullptr;
UdpDataService* UdpDataService::getInstance(QObject *parent)
{
    if(m_instance == nullptr) {
        m_instance = new UdpDataService(parent);
    }

    return m_instance;
}

UdpDataService::UdpDataService(QObject *parent) : QObject(parent)
{
    m_itemsController = MapItemsController::getInstance(this);
    connect(this, &UdpDataService::moveableChanged, m_itemsController, &MapItemsController::setItemsMoveable);
}

void UdpDataService::parseReceivedData(QByteArray data)
{
    ReceiveData_t receiveData;
    double *rawData = reinterpret_cast<double*>(data.data());

    receiveData.m_velocity          = *rawData++;
    receiveData.m_direction         = static_cast<ReceiveData_t::Direction>(static_cast<int>(*rawData++));
    receiveData.m_regulatorWorks    = static_cast<bool>(*rawData);


    if(receiveData.m_regulatorWorks) {
        m_itemsController->moveRobot(receiveData, qRound(5 * receiveData.m_velocity));
    }
}

void UdpDataService::prepareDataToSend(SensorData_t &sensorData)
{
    SendData_t dataToSend;

    dataToSend.m_leftDistance       = sensorData.topDistance;
    dataToSend.m_straightDistance   = sensorData.straightDistance;
    dataToSend.m_rightDistance      = sensorData.bottomDistance;
    dataToSend.m_distanceToEnd      = m_itemsController->distanceToEnd();
    dataToSend.m_stopSimulation     = static_cast<double>(m_itemsController->metaAchieved());   

    if(outOfTheMap(dataToSend)) {
        dataToSend.m_stopSimulation = 1; // robot out of the map range
    }
    // prepare buffer to send
    QByteArray data;
    QDataStream str{&data, QIODevice::WriteOnly};
    str << dataToSend;

    emit sendData(data);

    if(dataToSend.m_stopSimulation > 0) {
        emit simulationFinished();
        emit moveableChanged(true);
    }
}

bool UdpDataService::outOfTheMap(const SendData_t &sendData)
{
    return false;//(sendData.m_leftDistance <= 0.005 || sendData.m_straightDistance <= 0.005 || sendData.m_rightDistance <= 0.005); // &&
//            (sendData.m_rightDistance == 0 && sendData.m_leftDistance >= 0.95 && sendData.m_straightDistance >= 0.95) &&
//            (sendData.m_straightDistance == 0 && sendData.m_rightDistance >= 0.95 && sendData.m_leftDistance >= 0.95);
}
