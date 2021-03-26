#include "obstaclescontainer.h"
#include <QDebug>
#include <QQuickView>

ObstaclesContainer::ObstaclesContainer(QObject *parent) :
    QObject(parent)
{
}

void ObstaclesContainer::addObstacles(QVector<QQuickItem *> obstacles)
{
    clear();
    m_obstacles.append(obstacles);
}

QVector<QQuickItem *>& ObstaclesContainer::obstacles()
{
    return m_obstacles;
}

void ObstaclesContainer::setMoveable(bool moveable)
{
    for(auto &obstacle : m_obstacles) {
        obstacle->setProperty("moveable", moveable);
    }
}

void ObstaclesContainer::addObstacle(QQuickItem *obstacle)
{
    m_obstacles.push_back(obstacle);
    qDebug() << "obstacle added";
}

void ObstaclesContainer::clear()
{
    qDeleteAll(m_obstacles);
    m_obstacles.clear();

    qDebug() << "obstacled destroyed";
}
