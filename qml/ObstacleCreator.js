.pragma library

var obstacleComponent;
var obstacle;

function createObstacle(parent, args) {
    obstacleComponent   = Qt.createComponent("qrc:/qml/obstacle_course/items/ObstacleInnerItem.qml")
    obstacle            = obstacleComponent.createObject(parent, args)

    if(obstacle === null) {
        throw new Error("Cannot create instance for:" + parent)
    }

    return obstacle    
}
