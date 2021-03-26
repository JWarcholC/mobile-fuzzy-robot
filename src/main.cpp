#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/udp/udpsocket.h"
#include "src/ultrasonic_sensor/ultrasonic_object/ultrasonicalobstacledetector.h"
#include "src/obstacles/obstaclemanager.h"
#include "src/map_item/mapitemscontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("KI"));
    app.setOrganizationDomain(QStringLiteral("pl.edu.pwsztar"));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    qmlRegisterSingletonType(QUrl("qrc:/qml/Style.qml"), "Style", 1, 0, "Style");
    qmlRegisterSingletonType<ObstacleManager>("CppObjects", 1, 0, "ObstacleManager", [&app, &engine](QQmlEngine*, QJSEngine*) {
        auto obstaclesManager = new ObstacleManager(&engine, &app);
        return obstaclesManager;
    });
    qmlRegisterSingletonType<MapItemsController>("CppObjects", 1, 0, "MapItemsController", [&app](QQmlEngine*, QJSEngine*) {
        auto itemsController = MapItemsController::getInstance(&app);
        return itemsController;
    });
    qmlRegisterSingletonType<QObject>("CppConstant", 1, 0, "FilenameCharacterCut", [&app](QQmlEngine*, QJSEngine*) {
        QObject *obj = new QObject(&app);
        obj->setProperty("objectName",
                 #ifdef Q_OS_UNIX
                         QString::number(7) // xD
                 #else
                         QString::number(8) // WTF
                 #endif
                         );

        return obj;
    });
    qmlRegisterType<UdpSocket>("CppObjects", 1, 0, "UdpSocket");
    qmlRegisterType<UltrasonicalObstacleDetector>("CppObjects.Ultrasonic", 1, 0, "UltrasonicDetector");

    engine.load(url);
    return app.exec();
}
