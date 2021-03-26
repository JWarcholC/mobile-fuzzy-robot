import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0

Popup {
    id: root
    property real angle: 0.

    onVisibleChanged: {
        if(visible) {
            angleAnimation.start()
        } else {
            angleAnimation.stop()
        }
    }

    NumberAnimation {
        id: angleAnimation
        loops: Animation.Infinite
        duration: 800

        target: root
        property: "angle"
        from: 0.
        to: 360.
    }

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.horizontalCenter: parent.horizontalCenter

        wrapMode: Text.WordWrap
        text: qsTr("Made by students of State Higher Vocational School in Tarnow, A.D. 2020/2021.")
        font {
            pointSize: 12
            italic: true
        }
    }

    Image {
        id: img
        width: 200
        height: width
        anchors.centerIn: parent
        source: "qrc:/resources/images/ki"
    }

    ShaderEffect {
        anchors.fill: img
        property variant source: ShaderEffectSource {
            sourceItem: img
            hideSource: true
        }
        property real angle: root.angle

        fragmentShader: "qrc:/resources/shaders/about/frag_shader"
    }
}
