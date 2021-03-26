#ifndef MAPITEMSCONTROLLER_H
#define MAPITEMSCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include "src/udp/data/receivedata_t.h"

class MapItemsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* robotItem READ robotItem WRITE setRobotItem NOTIFY robotItemChanged)
    Q_PROPERTY(QQuickItem* metaItem READ metaItem WRITE setMetaItem NOTIFY metaItemChanged)

public:
    static MapItemsController* getInstance(QObject *parent = nullptr);

    void moveRobot(const ReceiveData_t& receiveData, int offset);
    double distanceToEnd() const;
    bool metaAchieved() const;
    QQuickItem* robotItem() const;
    QQuickItem* metaItem() const;

public slots:
    void setRobotItem(QQuickItem* robotItem);
    void setMetaItem(QQuickItem* metaItem);
    void setItemsMoveable(bool moveable);

signals:
    void robotItemChanged(QQuickItem* robotItem);
    void metaItemChanged(QQuickItem* metaItem);

private:
    static MapItemsController *m_instance;
    explicit MapItemsController(QObject *parent = nullptr);

    QQuickItem* m_robotItem;
    QQuickItem* m_metaItem;
    double m_ratio;
};

#endif // MAPITEMSCONTROLLER_H
