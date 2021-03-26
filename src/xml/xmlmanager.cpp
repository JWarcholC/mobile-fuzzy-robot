#include "xmlmanager.h"
#include <QDateTime>
#include <QDebug>

XmlManager::XmlManager(QObject *parent) :
    QObject(parent),
    m_xmlReader(new QXmlStreamReader),
    m_xmlWriter(new QXmlStreamWriter)
{}

void XmlManager::writeTo(const QString &device, QVector<QQuickItem*>& obstacles, QVector<QPointF>& coords)
{
    QFile file{device};
    file.open(QIODevice::WriteOnly);
    m_xmlWriter->setDevice(&file);
    m_xmlWriter->setAutoFormatting(true);

    qDebug() << "XML writing started";

    m_xmlWriter->writeStartDocument();
    m_xmlWriter->writeStartElement(QStringLiteral("content"));

    // write file info
    m_xmlWriter->writeStartElement(QStringLiteral("file"));

    m_xmlWriter->writeTextElement(QStringLiteral("author"), getenv("USER"));
    m_xmlWriter->writeTextElement(QStringLiteral("creationDate"), QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));

    m_xmlWriter->writeEndElement(); // end <file>

    // write robot and meta position
    m_xmlWriter->writeStartElement(QStringLiteral("mapItems"));

    m_xmlWriter->writeStartElement(QStringLiteral("robot"));
    m_xmlWriter->writeTextElement(QStringLiteral("topLeftX"), QString::number(coords.at(0).x()));
    m_xmlWriter->writeTextElement(QStringLiteral("topLeftY"), QString::number(coords.at(0).y()));
    m_xmlWriter->writeEndElement(); // end <robot>

    m_xmlWriter->writeStartElement(QStringLiteral("meta"));
    m_xmlWriter->writeTextElement(QStringLiteral("topLeftX"), QString::number(coords.at(1).x()));
    m_xmlWriter->writeTextElement(QStringLiteral("topLeftY"), QString::number(coords.at(1).y()));
    m_xmlWriter->writeEndElement(); // end <meta>

    m_xmlWriter->writeEndElement(); // end <mapItems>

    // write obstacles
    m_xmlWriter->writeStartElement(QStringLiteral("obstacles"));

    m_xmlWriter->writeTextElement("count", QString::number(obstacles.length()));
    for(const auto &obstacle : obstacles) {
        // begin write obstacle
        m_xmlWriter->writeStartElement(QStringLiteral("obstacle"));

        // write property
        m_xmlWriter->writeTextElement(QStringLiteral("topLeftX"), QString::number(obstacle->x()));
        m_xmlWriter->writeTextElement(QStringLiteral("topLeftY"), QString::number(obstacle->y()));
        m_xmlWriter->writeTextElement(QStringLiteral("rotation"), QString::number(obstacle->rotation()));

        // close write
        m_xmlWriter->writeEndElement(); // end <obstacle>
    }

    m_xmlWriter->writeEndElement(); // end <obstacles>

    m_xmlWriter->writeEndElement(); // end <content>
    m_xmlWriter->writeEndDocument();
    file.close();

    qDebug() << "XML writing finished";
}

void XmlManager::readFrom(const QString &device)
{
    QFile file{device};
    if(!(file.exists() && file.open(QIODevice::ReadOnly))) {
        qDebug() << "Cannot open file: " << device;
        return;
    }

    QVector<QQuickItem*> obstacles;
    QVector<QPointF> coords;

    qDebug() << "XML reading started";
    m_xmlReader->setDevice(&file);
    if(m_xmlReader->readNextStartElement()) {
        if(m_xmlReader->name() == QStringLiteral("content")) {
            while(m_xmlReader->readNextStartElement()) {
                if(m_xmlReader->name() == QStringLiteral("mapItems")) {
                    while(m_xmlReader->readNextStartElement()) {
                        if(m_xmlReader->name() == QStringLiteral("robot")) {

                            QPointF point;
                            while(m_xmlReader->readNextStartElement()) {
                                if(m_xmlReader->name() == QStringLiteral("topLeftX")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    point.setX(value);

                                } else if(m_xmlReader->name() == QStringLiteral("topLeftY")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    point.setY(value);
                                } else {
                                    m_xmlReader->skipCurrentElement();
                                }
                            }

                            coords.push_back(point);
                        } else if(m_xmlReader->name() == QStringLiteral("meta")) {

                            QPointF point;
                            while(m_xmlReader->readNextStartElement()) {
                                if(m_xmlReader->name() == QStringLiteral("topLeftX")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    point.setX(value);
                                } else if(m_xmlReader->name() == QStringLiteral("topLeftY")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    point.setY(value);
                                } else {
                                    m_xmlReader->skipCurrentElement();
                                }
                            }

                            coords.push_back(point);
                        } else {
                            m_xmlReader->skipCurrentElement();
                        }
                    }

                } else if(m_xmlReader->name() == QStringLiteral("obstacles")) {
                    while(m_xmlReader->readNextStartElement()) {
                        if(m_xmlReader->name() == QStringLiteral("count")) {
                            auto value = m_xmlReader->readElementText().toInt();
                            obstacles.reserve(value);
                        } else if(m_xmlReader->name() == QStringLiteral("obstacle")) {

                            QQuickItem *obstacle = new QQuickItem();
                            while(m_xmlReader->readNextStartElement()) {
                                if(m_xmlReader->name() == QStringLiteral("topLeftX")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    obstacle->setX(value);
                                } else if(m_xmlReader->name() == QStringLiteral("topLeftY")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    obstacle->setY(value);
                                } else if(m_xmlReader->name() == QStringLiteral("rotation")) {
                                    auto value = m_xmlReader->readElementText().toDouble();
                                    obstacle->setRotation(value);
                                } else {
                                    m_xmlReader->skipCurrentElement();
                                }
                            }

                            obstacles.push_back(obstacle);
                        } else {
                            m_xmlReader->skipCurrentElement();
                        }
                    }
                } else {
                    m_xmlReader->skipCurrentElement();
                }
            }
        } else {
            qDebug() << "Incorrect file";
            return;
        }
    }

    qDebug() << "obstacles:" << obstacles;
    qDebug() << "map items:" << coords;
    qDebug() << "XML reading finished";

    emit xmlParsed(obstacles, coords);
}
