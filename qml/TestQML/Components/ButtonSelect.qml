// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5

Rectangle {
  width: 400
  height: 30
  id: container

  signal clicked;

  property alias text: button_text.text
  property alias value: value_text.text

  property bool showTop;
  property bool showBottom;

  Text {
    id: button_text
    anchors.fill: parent
    text: qsTr("Button")
  }

  Text {
    id: value_text
    anchors.fill: parent
    horizontalAlignment: Text.AlignRight;
    text: ""
  }

  MouseArea {
      id: mouseArea
      anchors.fill: parent
      onClicked: container.clicked()
  }

}
