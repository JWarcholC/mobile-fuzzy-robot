#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H

#include <QObject>
#include <QPointF>

// forward declaration
class XmlManager;
class ObstaclesContainer;
class UdpDataService;
class QQmlEngine;
class QQuickItem;

class ObstacleManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* mapItem READ mapItem WRITE setMapItem NOTIFY mapItemChanged)

public:
    explicit ObstacleManager(QQmlEngine *engine, QObject *parent = nullptr);
    QQuickItem* mapItem() const;

public slots:
    void setMapItem(QQuickItem* mapItem);
    void setObstacleMoveable(bool moveable);

signals:
    void readRequested(QString device);
    void writeRequested(QString device, QVector<QPointF> coords);

    void obstacleAdded(QQuickItem *obstacle);
    void obstaclesCleared();

    void mapItemChanged(QQuickItem* mapItem);

private slots:
    void updateMap(QVector<QQuickItem *> &obstacles, QVector<QPointF> &coords);

private:
    XmlManager *m_xmlManager;
    ObstaclesContainer *m_obstaclesContainer;
    UdpDataService *m_udpService;
    QQuickItem* m_mapItem;
    QQmlEngine * m_engine;
};

#endif // OBSTACLEMANAGER_H
