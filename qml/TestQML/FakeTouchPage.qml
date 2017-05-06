import QtQuick 2.5

Item {
    id: root

    signal clicked()

    MouseArea {
        anchors.fill: parent

        onClicked: backend.switchScreen(true)
    }
}
