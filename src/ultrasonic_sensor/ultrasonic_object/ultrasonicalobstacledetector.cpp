#include "ultrasonicalobstacledetector.h"
#include <QDebug>
#include <QTimer>

#include "../sensordata_t.h"

UltrasonicalObstacleDetector::UltrasonicalObstacleDetector(QQuickItem *parent) :
    QQuickItem(parent),
    m_timer(new QTimer(this)),
    m_mapItem(nullptr),
    m_obstacleWidth(1),
    m_obstacleHeight(1),
    m_interval(1000)
{
    m_udpService = UdpDataService::getInstance();

    connect(this, &UltrasonicalObstacleDetector::sensorDataReceived, m_udpService, &UdpDataService::prepareDataToSend);
    connect(m_timer, &QTimer::timeout, this, &UltrasonicalObstacleDetector::findCollisions, Qt::QueuedConnection);
    connect(m_udpService, &UdpDataService::simulationFinished, this, &UltrasonicalObstacleDetector::stopObstacleDetection);
}

QQuickItem *UltrasonicalObstacleDetector::mapItem() const
{
    return m_mapItem;
}

int UltrasonicalObstacleDetector::obstacleWidth() const
{
    return m_obstacleWidth;
}

int UltrasonicalObstacleDetector::obstacleHeight() const
{
    return m_obstacleHeight;
}

int UltrasonicalObstacleDetector::interval() const
{
    return m_interval;
}

void UltrasonicalObstacleDetector::startObstacleDetection()
{
    if(m_timer->isActive()) {
        m_timer->stop();
    }

    m_timer->start(m_interval);
    QMetaObject::invokeMethod(m_udpService, "moveableChanged", Qt::QueuedConnection, Q_ARG(bool, false));
}

void UltrasonicalObstacleDetector::stopObstacleDetection()
{
    m_timer->stop();
    emit detectionFinished();
}

void UltrasonicalObstacleDetector::setMapItem(QQuickItem *mapItem)
{
    if (m_mapItem == mapItem)
        return;

    m_mapItem = mapItem;
    emit mapItemChanged(m_mapItem);
}

void UltrasonicalObstacleDetector::setObstacleWidth(int obstacleWidth)
{
    if (m_obstacleWidth == obstacleWidth)
        return;

    m_obstacleWidth = obstacleWidth;
    emit obstacleWidthChanged(m_obstacleWidth);
}

void UltrasonicalObstacleDetector::setObstacleHeight(int obstacleHeight)
{
    if (m_obstacleHeight == obstacleHeight)
        return;

    m_obstacleHeight = obstacleHeight;
    emit obstacleHeightChanged(m_obstacleHeight);
}

void UltrasonicalObstacleDetector::setInterval(int interval)
{
    if (m_interval == interval)
        return;

    m_interval = interval;
    emit intervalChanged(m_interval);
}

void UltrasonicalObstacleDetector::findCollisions()
{
    auto currentX = static_cast<int>(x());
    auto currentY = static_cast<int>(y());

    auto straightDetectorRange  = static_cast<int>(m_mapItem->width());
    auto bottomDetectorRange    = static_cast<int>(m_mapItem->height());
    auto topDetectorRange       = 0;

    auto loopStep = (m_obstacleWidth / 2 - 1);
    QVector<QQuickItem*> obstacles;

    SensorData_t sensorData;

    // find obstacle straight
    int i;
    QQuickItem *child = nullptr;
    for(i = currentX; i <= straightDetectorRange; i += loopStep) {
        child = m_mapItem->childAt(i, currentY);
        if(child != nullptr) {
            if(child->property("isObstacle").toBool()) {
                if(!obstacles.contains(child)) {

                    obstacles.push_back(child);
                    break;
                }
            }
        }
    }
    sensorData.straightDistance = static_cast<double>(1. * (i - currentX) / (straightDetectorRange - currentX));

    // find obstacle top
    child = nullptr;
    for(i = currentY; i >= topDetectorRange; i -= loopStep) {
        child = m_mapItem->childAt(currentX, i);
        if(child != nullptr) {
            if(child->property("isObstacle").toBool()) {
                if(!obstacles.contains(child)) {

                    obstacles.push_back(child);
                    break;
                }
            }
        }
    }
    sensorData.topDistance = static_cast<double>(1. * (currentY - i) / (currentY));

    // find obstacle bottom
    child = nullptr;
    for(i = currentY; i < bottomDetectorRange; i += loopStep) {
        child = m_mapItem->childAt(currentX, i);
        if(child != nullptr) {
            if(child->property("isObstacle").toBool()) {
                if(!obstacles.contains(child)) {

                    obstacles.push_back(child);
                    break;
                }
            }
        }
    }
    sensorData.bottomDistance = static_cast<double>(1. * (i - currentY) / (bottomDetectorRange - currentY));

    emit sensorDataReceived(sensorData);
    qDebug() << "sensor:" << "straight:" << sensorData.straightDistance << "\ttop:" << sensorData.topDistance << "\tbottom:" << sensorData.bottomDistance;
}
