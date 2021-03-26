#include "obstaclemanager.h"

#include "src/xml/xmlmanager.h"
#include "src/udp/data_service/udpdataservice.h"
#include "obstaclescontainer.h"
#include <QQmlEngine>

ObstacleManager::ObstacleManager(QQmlEngine *engine, QObject *parent) :
    QObject(parent),
    m_mapItem(nullptr),
    m_engine(engine)
{
    m_xmlManager            = new XmlManager(this);
    m_obstaclesContainer    = new ObstaclesContainer(this);
    m_udpService            = UdpDataService::getInstance();

    connect(this, &ObstacleManager::obstacleAdded, m_obstaclesContainer, &ObstaclesContainer::addObstacle);
    connect(this, &ObstacleManager::obstaclesCleared, m_obstaclesContainer, &ObstaclesContainer::clear);

    connect(m_xmlManager, &XmlManager::xmlParsed, this, &ObstacleManager::updateMap);
    connect(m_udpService, &UdpDataService::moveableChanged, this, &ObstacleManager::setObstacleMoveable);
    connect(this, &ObstacleManager::readRequested, m_xmlManager, &XmlManager::readFrom);
    connect(this, &ObstacleManager::writeRequested, this, [this](QString device, QVector<QPointF> coords) {
        m_xmlManager->writeTo(device, m_obstaclesContainer->obstacles(), coords);
    });
}

QQuickItem *ObstacleManager::mapItem() const
{
    return m_mapItem;
}

void ObstacleManager::setMapItem(QQuickItem *mapItem)
{
    if (m_mapItem == mapItem)
        return;

    m_mapItem = mapItem;
    emit mapItemChanged(m_mapItem);
}

void ObstacleManager::setObstacleMoveable(bool moveable)
{
    m_obstaclesContainer->setMoveable(moveable);
}

void ObstacleManager::updateMap(QVector<QQuickItem *> &obstacles, QVector<QPointF> &coords)
{

    // set map items
    if(m_mapItem) {
        auto robotCoords = coords.at(0);
        auto metaCoords  = coords.at(1);

        auto robotItem  = qvariant_cast<QObject*>(m_mapItem->property("robotItem"));
        auto metaItem   = qvariant_cast<QObject*>(m_mapItem->property("metaItem"));

        robotItem->setProperty("x", robotCoords.x());
        robotItem->setProperty("y", robotCoords.y());

        metaItem->setProperty("x", metaCoords.x());
        metaItem->setProperty("y", metaCoords.y());
    }

    // set obstacles
    QQmlComponent component{m_engine, QUrl("qrc:/qml/obstacle_course/items/ObstacleInnerItem.qml")};
    QVector<QQuickItem*> visibleObstacles;
    visibleObstacles.reserve(obstacles.size());

    for(auto obstacle : obstacles) {
        obstacle->setParent(m_mapItem);

        auto obstacleItem = qobject_cast<QQuickItem*>(component.create());
        obstacleItem->setParent(m_mapItem);
        obstacleItem->setParentItem(m_mapItem); // to make item visible in qml

        // set property
        obstacleItem->setX(obstacle->x());
        obstacleItem->setY(obstacle->y());
        obstacleItem->setRotation(obstacle->rotation());
        obstacleItem->setWidth(80);
        obstacleItem->setHeight(200);

        obstacleItem->setProperty("obstacleOnMap", true);
        obstacleItem->setProperty("obstacleMap", QVariant::fromValue(m_mapItem));

        visibleObstacles.push_back(obstacleItem);
        delete obstacle;
    }

    m_obstaclesContainer->addObstacles(visibleObstacles);
}
