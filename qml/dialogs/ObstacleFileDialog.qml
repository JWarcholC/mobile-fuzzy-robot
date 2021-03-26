import QtQuick 2.12
import QtQuick.Dialogs 1.0
import CppConstant 1.0

Item {
    id: root
    property alias title: fileDialog.title
    property alias filters: fileDialog.nameFilters
    property alias openFile: fileDialog.selectExisting

    signal accepted(string fileName)
    signal rejected()

    function open() {
        fileDialog.open()
    }

    function close() {
        root.destroy()
    }

    FileDialog {
        id: fileDialog
        visible: false

        modality: Qt.NonModal
        nameFilters: ["Data files (*.xml)" , "All files (*)"]
        folder: shortcuts.home

        onAccepted: {
            var file = fileUrl.toString().substring(FilenameCharacterCut.objectName)
            console.log("file:", file)

            root.accepted(file)
            fileDialog.close()
        }

        onRejected: {
            root.rejected()
            fileDialog.close()
        }
    }
}
