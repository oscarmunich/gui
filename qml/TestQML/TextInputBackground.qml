// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../js/Styler.js" as Styler

Rectangle {
  id: container
  width: 100
  height: 62
  radius: 6
  color: "white";

  border.width: 2
  border.color: "gray"

  property bool useDefaultKeyboard: true
  property bool numeric: false
  property alias passwordCharacter: textInput2.passwordCharacter
  property alias echoMode: textInput2.echoMode
  property alias text: textInput2.text
  property alias inputMask: textInput2.inputMask
  property alias validator: textInput2.validator

  signal gotFocus(bool focus)

  TextInput {
    id: textInput2
    anchors.rightMargin: 4
    anchors.leftMargin: 4
//    anchors.bottomMargin: 4
//    anchors.topMargin: 4
//    anchors.fill: parent
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter

    font {
        family: Styler.Fonts.Family
        pixelSize: Styler.Fonts.ItemTitle.PixelSize
    }

    onFocusChanged: {
        if (useDefaultKeyboard && focus) {
            keyboard.visible = true;

            if (numeric)
                keyboard.showNumbers();
            else
                keyboard.showAbc();
        }

        container.gotFocus(focus);
    }
  }
}
