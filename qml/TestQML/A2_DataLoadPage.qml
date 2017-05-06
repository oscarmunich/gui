import QtQuick 2.5
import "./Components"

Page {
    id: root

    signal swpnMenuSelection()
    signal swpnManualSelection()

    Column {
        anchors {
            top: parent.top
            topMargin: 30
            horizontalCenter: parent.horizontalCenter
        }

        spacing: 15

        HeaderText {
            text: "Part Number Selection"
            visible: backend.getA6153Enabled()
        }

        GroupBox {
            radius: 10
            border.width: 2
            border.color: "blue"
            visible: backend.getA6153Enabled()
            color: "lightblue"

            MenuItem {
                text: "Menu selection"

                onClicked: root.swpnMenuSelection()
            }
            MenuItem {
                text: "Manual selection"

                onClicked: root.swpnManualSelection()
            }
        }
    }
}
