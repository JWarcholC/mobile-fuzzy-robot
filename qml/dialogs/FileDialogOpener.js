.pragma library

var _popUpComponent = null
var _popUp = null

function open(file, parent, args) {
    if(_popUpComponent === null) {
        _popUpComponent = Qt.createComponent(file)
    }

    _popUp = _popUpComponent.createObject(parent, args)
    if(_popUp === null) {
        console.error("FileDialogOpener: cannot create instance for:", file)
    }

    return _popUp
}
