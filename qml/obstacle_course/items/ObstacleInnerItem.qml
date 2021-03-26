import QtQuick 2.12
import Style 1.0

Item {
    id: root
    property alias moveable: mouseArea.enabled
    property bool obstacleOnMap: false
    property var obstacleMap: null
    property bool isObstacle: true

    Drag.active: root.obstacleOnMap
    Drag.source: mouseArea

    Rectangle {
        id: rect
        width: 10
        height: 110
        anchors.centerIn: parent

        color: Style.obstacleColor
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        drag {
            target: root
            axis: Drag.XAndYAxis

            minimumX: 0
            maximumX: obstacleMap === null ? 0 : obstacleMap.width// - root.width * 0.3
                                                                  //
            minimumY: 0                                           //
            maximumY: obstacleMap === null ? 0 : obstacleMap.height// - root.height * 0.3
        }
    }
}
