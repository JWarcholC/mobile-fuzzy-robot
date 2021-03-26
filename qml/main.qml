import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12

import CppObjects 1.0
import Style 1.0
import "qrc:/qml/obstacle_course"
import "qrc:/qml/dialogs"
import "qrc:/"

ApplicationWindow {
    id: window

    width: 1400
    height: 800
    minimumWidth: 800
    minimumHeight: 750
    visible: true
    title: qsTr("Fuzzy Robot Map")

    property alias aboutPopup: aboutPopup

    UdpSocket {
        id: udp

        onNewData: {
            udpData.text += datagram + '\n'
        }
    }

    menuBar: ObstacleMenuBar {
        robot: obstacleCourse.robot
        meta: obstacleCourse.meta
    }

    header: Rectangle {
        id: header
        height: 60
        color: Style.mainColor

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            spacing: 20

            Label {
                font.pointSize: 12
                text: qsTr("Simulink address")
            }

            TextField {
                id: addrFld

                Layout.fillWidth: true

                placeholderText: qsTr("Localhost if empty...")

                validator: RegExpValidator {
                    regExp: /(\d{1,3}[.]){3}\d{1,3}/
                }
                onTextChanged: acceptableInput ? udp.hostAddress = text : ""
            }

            Label {
                font.pointSize: 12
                text: qsTr("Port")
            }

            TextField {
                id: portFld
                placeholderText: qsTr("Simulink send port number")

                validator: RegExpValidator {
                    regExp: /\d{1,5}/
                }

                onTextChanged: acceptableInput ? udp.hostPort = text : ""
            }

            Button {
                id: btn

                text: qsTr("Start UDP")
                enabled: portFld.acceptableInput

                onReleased: udp.initSocket()

                Connections {
                    target: udp

                    function onConnectionBound() {
                        btn.text = qsTr("Connected")
                        btn.enabled = false
                    }
                }
            }
        }
    }

    ObstacleCourse {
        id: obstacleCourse
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    About {
        id: aboutPopup
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        width: 400
        height: width
        visible: false
    }

    footer: Rectangle {
        id:footer
        height: 200
        color: Style.mainColor

        Rectangle {
            anchors.fill: parent
            anchors.margins: 10
            color: "transparent"
            radius: 5
            border {
                width: 1
                color: "black"
            }


            Flickable {
                id: flickable
                anchors.fill: parent
                anchors.margins: 10
                contentWidth: udpData.width;
                contentHeight: udpData.height
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                clip: true

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                }

                TextArea {
                    id: udpData
                    height: 500
                    width: window.width

                    readOnly: true
                    wrapMode: Text.WordWrap
                    placeholderText: qsTr("UDP logs...")

                    onTextChanged: height = height + 10
                }
            }
        }
    }
}
