import QtQuick 2.12

import "qrc:/qml/obstacle_course/items"

Item {
    id: root
    property var grabbedItem: null

    ObstacleInnerItem {
        id: obstacleItem
        width: parent.width
        height: parent.height
    }

    Drag.active: mouseArea.drag.active
    Drag.dragType: Drag.Automatic
    Drag.supportedActions: Qt.CopyAction
    Drag.mimeData: {
        "rotation" : rotation
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        drag {
            target: parent
            axis: Drag.XAndYAxis
        }

        onPressed: {
                grabbedItem.grabToImage(function(res) {
                    parent.Drag.imageSource = res.url
                })
        }
    }
}
