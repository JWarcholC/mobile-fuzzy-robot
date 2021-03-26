#ifndef XMLMANAGER_H
#define XMLMANAGER_H

#include <QObject>
#include <QQuickItem>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class XmlManager : public QObject
{
    Q_OBJECT
public:
    explicit XmlManager(QObject *parent = nullptr);

signals:
    void xmlParsed(QVector<QQuickItem*> &obstacles, QVector<QPointF> &coords);

public slots:
    void writeTo(const QString &device, QVector<QQuickItem *> &obstacles, QVector<QPointF> &coords);
    void readFrom(const QString &device);

private:
    QScopedPointer<QXmlStreamReader> m_xmlReader;
    QScopedPointer<QXmlStreamWriter> m_xmlWriter;
};

#endif // XMLMANAGER_H
