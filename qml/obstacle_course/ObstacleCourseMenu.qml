import QtQuick 2.0
import QtQuick.Controls 2.3

import Style 1.0
import "qrc:/qml/obstacle_course/items"

Item {
    id: root
    property int itemWidth: parent.width - 20

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 2

        Button {
            id: startBtn

            property var btnColor: Style.buttonColor

            width: itemWidth
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter

            function toggleState() {
                startBtn.enabled = !startBtn.enabled
                startBtn.opacity = startBtn.opacity === 1 ? 0.5 : 1
            }

            text: qsTr("Start")
            enabled: false
            opacity: 0.5

            background: Rectangle {
                id: dd
                color: startBtn.btnColor
                radius: 5
            }

            Behavior on opacity {
                PropertyAnimation {duration: 300}
            }

            Behavior on enabled {
                PropertyAnimation {duration: 300}
            }

            onClicked: {
                detector.startObstacleDetection()
                startBtn.toggleState()
            }

            Connections {
                target: detector
                function onDetectionFinished() {
                    startBtn.toggleState()
                }
            }

            Connections {
                target: udp
                function onConnectionBound() {
                    startBtn.toggleState()
                }
            }
        }

        Rectangle {
            height: 1
            width: itemWidth
            color: Style.mainColor
        }

        Item {
            width: 2 * grid.cellWidth + 10
            height: parent.height

            GridView {
                id: grid
                anchors.fill: parent
                anchors.topMargin: 15
                anchors.leftMargin: 7
                cellWidth: 135
                cellHeight: 135

                model: obstacleModel
                delegate: obstacleDelegate
            }

            ListModel {
                id: obstacleModel
                ListElement {rotationAngle : 0}
                ListElement {rotationAngle : 90}
                ListElement {rotationAngle : 45}
                ListElement {rotationAngle : 135}
            }

            Component {
                id: obstacleDelegate

                Rectangle {

                    color: "black"
                    width: 130
                    height: 130

                    Item { // it is used to grab correct image during dragging
                        id: obstacleWrapper
                        anchors.fill: parent

                        ObstacleItem {
                            id: obstacleItem
                            anchors.centerIn: parent
                            width: 80
                            height: 200

                            rotation: rotationAngle
                            grabbedItem: obstacleWrapper
                        }
                    }
                }
            }
        }
    }
}
