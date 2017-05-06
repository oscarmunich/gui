import QtQuick 2.5
import "../"

Rectangle {
    id: root

    signal accepted()
    signal rejected()

    visible: false

    function show(text, acceptText, rejectText) {
        messageText.text = text;
        acceptButton.text = acceptText;
        rejectButton.text = rejectText;

        root.visible = true;
    }

    width: 480
    height: 800
    color: "lightgray"

    Text {
        id: messageText

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: acceptButton.top
            margins: 10
        }

        font {
            pixelSize: 20
        }

        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: "Are you sure you want to quit?"
    }

    Button {
        id: acceptButton

        anchors {
            left: parent.left
            leftMargin: 20
            bottom: parent.bottom
            bottomMargin: 5
        }

        width: parent.width / 3
        height: 70

        text: qsTr("Yes")
        z: 2
        onClicked: {
            root.accepted();
            root.visible = false;
        }
    }

    Button {
        id: rejectButton

        anchors {
            right: parent.right
            rightMargin: 20
            bottom: parent.bottom
            bottomMargin: 5
        }
        z: 2
        width: parent.width / 3
        height: 70

        text: qsTr("No")

        onClicked: {
            root.rejected();
            root.visible = false;
        }
    }

    MouseArea {
        anchors.fill: parent
    }
}
