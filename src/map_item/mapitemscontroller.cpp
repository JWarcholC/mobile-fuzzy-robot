#include "mapitemscontroller.h"

MapItemsController* MapItemsController::m_instance = nullptr;
MapItemsController* MapItemsController::getInstance(QObject *parent)
{
    if(m_instance == nullptr) {
        m_instance = new MapItemsController(parent);
    }

    return m_instance;
}

void MapItemsController::moveRobot(const ReceiveData_t &receiveData, int offset)
{
    switch (receiveData.m_direction) {
    case ReceiveData_t::Straight:
        m_robotItem->setProperty("x", m_robotItem->x() + offset);
        break;
    case ReceiveData_t::Left:
        m_robotItem->setProperty("y", m_robotItem->y() - offset);
        break;
    case ReceiveData_t::Right:
        m_robotItem->setProperty("y", m_robotItem->y() + offset);
    }
}

double MapItemsController::distanceToEnd() const
{
    auto diff = (m_metaItem->y() - m_robotItem->y()) / m_robotItem->parentItem()->height();
    return diff;
}

bool MapItemsController::metaAchieved() const
{
    auto lessThan = [](double a, double b) {return a <= b;};

    auto xDistance = qAbs(m_robotItem->x() - m_metaItem->x());
    auto yDistance = qAbs(m_robotItem->y() - m_metaItem->y());

    return  (lessThan(xDistance, 10) && lessThan(yDistance, 50)) || // one coord is captured
            (lessThan(xDistance, 15) && lessThan(yDistance, 15)) || // both coords are captured
            (lessThan(xDistance, 50) && lessThan(yDistance, 10));  // one coord is captured
}

QQuickItem *MapItemsController::robotItem() const
{
    return m_robotItem;
}

QQuickItem *MapItemsController::metaItem() const
{
    return m_metaItem;
}

void MapItemsController::setRobotItem(QQuickItem *robotItem)
{
    if (m_robotItem == robotItem)
        return;

    m_robotItem = robotItem;
    m_ratio = m_robotItem->parentItem()->height();
    emit robotItemChanged(m_robotItem);
}

void MapItemsController::setMetaItem(QQuickItem *metaItem)
{
    if (m_metaItem == metaItem)
        return;

    m_metaItem = metaItem;
    emit metaItemChanged(m_metaItem);
}

void MapItemsController::setItemsMoveable(bool moveable)
{
    m_robotItem->setProperty("moveable", moveable);
    m_metaItem->setProperty("moveable", moveable);
}

MapItemsController::MapItemsController(QObject *parent) :
    QObject(parent),
    m_robotItem(nullptr),
    m_metaItem(nullptr)
{}
