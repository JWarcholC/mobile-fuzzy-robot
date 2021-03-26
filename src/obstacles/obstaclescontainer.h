#ifndef OBSTACLESCONTAINER_H
#define OBSTACLESCONTAINER_H

#include <QObject>
#include <QQuickItem>

class ObstaclesContainer : public QObject
{
    Q_OBJECT
public:
    explicit ObstaclesContainer(QObject *parent = nullptr);
    void addObstacles(QVector<QQuickItem*> obstacles);
    QVector<QQuickItem *> &obstacles();

    void setMoveable(bool moveable);

public slots:
    void addObstacle(QQuickItem *obstacle);
    void clear();

signals:
    void obstacleAdded(QQuickItem *obstacle);
    void obstaclesCleared();

private:
    QVector<QQuickItem*> m_obstacles;
};

#endif // OBSTACLESCONTAINER_H
