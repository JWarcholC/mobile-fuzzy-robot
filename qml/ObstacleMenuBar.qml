import QtQuick 2.12
import QtQuick.Controls 2.4

import CppObjects 1.0
import Style 1.0
import "qrc:/qml/dialogs"
import "qrc:/qml/dialogs/FileDialogOpener.js" as FileDialogOpener

MenuBar {
    contentHeight: 20

    property var robot: null
    property var meta: null

    QtObject {
        id: inner
        readonly property string dialogUrl: "qrc:/qml/dialogs/ObstacleFileDialog.qml"
    }

    Menu {
        title: qsTr("&File")

        MenuItem {
            text: qsTr("&Open...")
            onTriggered: {
                var dialog = FileDialogOpener.open(inner.dialogUrl, window, {
                                                       title: qsTr("Select file...")
                                                   })

                if(dialog !== null) {
                    dialog.open()
                    dialog.rejected.connect(() => dialog.close())
                    dialog.accepted.connect(fileName => {
                                                ObstacleManager.readRequested(fileName)
                                                dialog.close()
                                            })
                }
            }
        }

        MenuItem {
            text: qsTr("&Save...")
            onTriggered: {
                var dialog = FileDialogOpener.open(inner.dialogUrl, window, {
                                                       title: qsTr("Save file as..."),
                                                       openFile: false
                                                   })

                if(dialog !== null) {
                    dialog.open()
                    dialog.rejected.connect(() => dialog.close())
                    dialog.accepted.connect(fileName => {
                                                ObstacleManager.writeRequested(fileName, [Qt.point(robot.x, robot.y), Qt.point(meta.x, meta.y)])
                                                dialog.close()
                                            })
                }
            }
        }

        MenuSeparator{}

        MenuItem{
            text: qsTr("&Quit")
            onTriggered: Qt.quit()
        }
    }

    Menu {
        title: qsTr("&Edit")

        MenuItem {
            text: qsTr("&Clear obstacles")
            onTriggered: ObstacleManager.obstaclesCleared()
        }
    }

    Menu {
        title: qsTr("&Help")

        MenuItem {
            text: qsTr("&About")
            onTriggered: window.aboutPopup.open()
        }
    }
}
