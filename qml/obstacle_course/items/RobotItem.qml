import QtQuick 2.12

Item {
    id: root

    property alias moveable: mouseArea.enabled
    property var obstacleMeta: null
    property real movementDuration: detector.interval

    Image {
        anchors.centerIn: parent
        source: "qrc:/resources/robot/robot"
    }

    Behavior on x {
        PropertyAnimation { target: root; duration: root.movementDuration }
    }

    Behavior on y {
        PropertyAnimation { target: root; duration: root.movementDuration }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        drag {
            target: root
            axis: Drag.XAndYAxis

            minimumX: 0
            maximumX: root.parent.width - root.width

            minimumY: 0
            maximumY: root.parent.height - root.height
        }
    }
}
