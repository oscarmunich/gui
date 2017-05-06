// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {
  color: "white"
  width: 400
  height: 36;

  property alias text: id_text.text;
  property alias on: id_switch.on;

  Text {
    id: id_text;
    font {
        family: Styler.Fonts.Family
        pixelSize: Styler.Fonts.ItemTitle.PixelSize
        weight: Styler.Fonts.ItemTitle.Weight == "Bold" ? Font.Bold : Font.Normal
    }
    text: "";
    verticalAlignment: Text.AlignVCenter
    anchors.fill: parent;
    anchors.leftMargin: 10
  }

  Switch {
    id: id_switch
    y: 4
    anchors.right: parent.right
    anchors.rightMargin: 4
  }
}
