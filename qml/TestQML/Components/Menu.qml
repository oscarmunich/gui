// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5

Rectangle {
  id: container

  radius: 10
  border.width: 2
  border.color: "gray"

  width: 400
  height: 118

  color: "lightgray"

  Column {
    anchors.fill: parent;
    anchors.margins: 5
    spacing: 1

    Rectangle {
      width: 390
      height: 35
      color: "white"
      radius: 5
      Text {
        anchors.fill: parent;
        anchors.leftMargin: 10
        font.family: "Helvetica"
        font.pixelSize: 18

        text: "Menu option 1"
        verticalAlignment: Text.AlignVCenter
      }
      MouseArea {
        anchors.fill: parent

      }
    }

    Rectangle {
      width: 390
      height: 35
      color: "white"
      radius: 5
      Text {
        anchors.fill: parent;
        anchors.leftMargin: 10
        font.family: "Helvetica"
        font.pixelSize: 18

        text: "Menu option 2"
        verticalAlignment: Text.AlignVCenter
      }
    }

    Rectangle {
      width: 390
      height: 35
      color: "white"
      radius: 5
      Text {
        anchors.fill: parent;
        anchors.leftMargin: 10
        font.family: "Helvetica"
        font.pixelSize: 18

        text: "Menu option 3"
        verticalAlignment: Text.AlignVCenter
      }

    }
  }
}
