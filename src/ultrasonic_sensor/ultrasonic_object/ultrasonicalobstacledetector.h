#ifndef ULTRASONICALOBSTACLEDETECTOR_H
#define ULTRASONICALOBSTACLEDETECTOR_H

#include <QQuickItem>
#include "src/udp/data_service/udpdataservice.h"

class UltrasonicalObstacleDetector : public QQuickItem
{
    Q_OBJECT

    // map
    Q_PROPERTY(QQuickItem* mapItem READ mapItem WRITE setMapItem NOTIFY mapItemChanged)

    // obstacles width/height
    Q_PROPERTY(int obstacleWidth READ obstacleWidth WRITE setObstacleWidth NOTIFY obstacleWidthChanged)
    Q_PROPERTY(int obstacleHeight READ obstacleHeight WRITE setObstacleHeight NOTIFY obstacleHeightChanged)

    // detection interval
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

public:
    explicit UltrasonicalObstacleDetector(QQuickItem *parent = nullptr);

    QQuickItem* mapItem() const;
    int obstacleWidth() const;
    int obstacleHeight() const;
    int interval() const;

public slots:
    void startObstacleDetection();
    void stopObstacleDetection();

    void setMapItem(QQuickItem* mapItem);
    void setObstacleWidth(int obstacleWidth);
    void setObstacleHeight(int obstacleHeight);
    void setInterval(int interval);

signals:
    void mapItemChanged(QQuickItem* mapItem);
    void obstacleWidthChanged(int obstacleWidth);
    void obstacleHeightChanged(int obstacleHeight);
    void intervalChanged(int interval);

    void sensorDataReceived(SensorData_t &sensorData);

    void detectionFinished();

private slots:
    void findCollisions();

private:
    QTimer *m_timer;
    QQuickItem* m_mapItem;
    UdpDataService *m_udpService;
    int m_obstacleWidth;
    int m_obstacleHeight;
    int m_interval;
};

#endif // ULTRASONICALOBSTACLEDETECTOR_H
