import QtQuick 2.0
import Style 1.0

Item {
    id: root
    property alias color: innerRct.color

    Rectangle {
        id: innerRct
        anchors.fill: parent
        color: Style.mapColor
        radius: 2
    }
}
