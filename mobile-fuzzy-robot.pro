QT += quick core network widgets quickcontrols2

CONFIG += c++17

# no debug in elease mode
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
        src/main.cpp \
        src/map_item/mapitemscontroller.cpp \
        src/obstacles/obstaclemanager.cpp \
        src/obstacles/obstaclescontainer.cpp \
        src/udp/data_service/udpdataservice.cpp \
        src/udp/udpsocket.cpp \
        src/ultrasonic_sensor/ultrasonic_object/ultrasonicalobstacledetector.cpp \
        src/xml/xmlmanager.cpp

RESOURCES += qml.qrc \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/map_item/mapitemscontroller.h \
    src/obstacles/obstaclemanager.h \
    src/obstacles/obstaclescontainer.h \
    src/udp/data/receivedata_t.h \
    src/udp/data/senddata_t.h \
    src/udp/data_service/udpdataservice.h \
    src/udp/udpsocket.h \
    src/ultrasonic_sensor/sensordata_t.h \
    src/ultrasonic_sensor/ultrasonic_object/ultrasonicalobstacledetector.h \
    src/xml/xmlmanager.h

DISTFILES += \
    qtquickcontrols2.conf
