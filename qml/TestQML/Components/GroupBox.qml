import QtQuick 2.5

Rectangle {
    id: root

    default property alias children: itemsColumn.children
    property alias spacing: itemsColumn.spacing

    width: itemsColumn.childrenRect.width + 10
    height: itemsColumn.height + 10

    Column {
        id: itemsColumn
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
            margins: 5
        }

        spacing: 1
    }
}
