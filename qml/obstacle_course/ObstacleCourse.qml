import QtQuick 2.12
import QtQuick.Layouts 1.3
import CppObjects 1.0
import CppObjects.Ultrasonic 1.0

import Style 1.0
import "qrc:/qml/ObstacleCreator.js" as ObstacleCreator
import "qrc:/qml/obstacle_course"
import "qrc:/qml/obstacle_course/items"

Item {
    id: root
    property alias robot: map.robotItem
    property alias meta: map.metaItem

    Component.onCompleted: {
        ObstacleManager.mapItem = map

        MapItemsController.robotItem = robot
        MapItemsController.metaItem  = meta
    }

    RowLayout {
        anchors.fill: parent
        anchors.topMargin: 60
        anchors.bottomMargin: 60
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 30

        ObstacleCourseMap {
            id: map
            Layout.fillHeight: true
            Layout.fillWidth: true

            property alias robotItem: robot
            property alias metaItem: meta

            DropArea {
                id: mapDropArea
                anchors.fill: parent
                enabled: true

                keys: "rotation"

                onDropped: {
                    if(drop.action === Qt.CopyAction) {
                        const formats = drop.formats
                        if(formats !== []) {
                            const rotationAngle = drop.getDataAsString(formats[0])
                            const point         = mapToItem(map, drop.x, drop.y)
                            const obstacle      = ObstacleCreator.createObstacle(map, {
                                                                          x: point.x,
                                                                          y: point.y,
                                                                          width: 80,
                                                                          height: 200,
                                                                          rotation: rotationAngle,
                                                                          obstacleOnMap: true,
                                                                          obstacleMap: map
                                                                      })

                            if(obstacle !== null) {
                                ObstacleManager.obstacleAdded(obstacle)
                            }
                        }
                    }

                }

                Rectangle {
                    id: dropRct
                    anchors.fill: parent

                    color: "transparent"

                    states: [
                        State {
                            when: mapDropArea.containsDrag
                            PropertyChanges {
                                target: dropRct
                                color: Qt.darker(map.color, 1.25)
                            }
                        }

                    ]
                }

            }

            RobotItem {
                id: robot
                implicitWidth: 100
                implicitHeight: 100

                x: 10
                y: 10
                z: 10

                obstacleMeta: meta

                UltrasonicDetector {
                    id: detector
                    x: parent.x + parent.width / 8
                    y: parent.y + parent.height / 8

                    mapItem: map
                    obstacleWidth: 10
                    obstacleHeight: 110
                    interval: 100
                }
            }

            ObstacleMetaItem {
                id: meta
                implicitWidth: 120
                implicitHeight: 120

                x: map.width - meta.width
                y: map.height - meta.height
                z: 0.1

                obstacleMap: map
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 300
            Layout.preferredHeight: 400
            Layout.minimumHeight: 355

            color: "transparent"
            radius: 10           

            border {
                color: Style.mainColor
                width: 2
            }

            ObstacleCourseMenu {
                id: mapMenu
                anchors.fill: parent
            }
        }
    }
}
