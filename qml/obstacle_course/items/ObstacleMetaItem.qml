import QtQuick 2.12

Item {
    id: root

    property alias moveable: mouseArea.enabled
    property var obstacleMap: null

    Image {
        anchors.centerIn: parent
        source: "qrc:/resources/map_items/meta"
    }

    Drag.active: true
    Drag.source: mouseArea

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property var minX: 0
        property var maxX: obstacleMap.width - root.width

        property var minY: 0
        property var maxY: obstacleMap.height - root.height

        property var dragTarget: root

        drag {
            target: mouseArea.dragTarget
            axis: Drag.XAndYAxis

            minimumX: mouseArea.minX
            maximumX: mouseArea.maxX

            minimumY: mouseArea.minY
            maximumY: mouseArea.maxY
        }
    }
}
