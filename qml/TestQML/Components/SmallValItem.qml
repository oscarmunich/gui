// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {
  property alias text: text_id.text
  property alias value: text_id_value.text
  property alias valuecol: text_id_value.color
  signal clicked

  id: container
  height: 32
  color: "lightblue"
  radius: 5
  SystemPalette { id: activePalette }

  gradient: Gradient {
      GradientStop {
          position: 0.0
          color: activePalette.light
      }
      GradientStop {
          position: 1.0
          color: activePalette.button
      }
  }

  Text {
    id: text_id
    anchors.fill: parent;
    anchors.leftMargin: 10
    font {
        family: Styler.Fonts.Family
        pixelSize: Styler.Fonts.ItemTitle.PixelSize
        weight: Styler.Fonts.ItemTitle.Weight == "Bold" ? Font.Bold : Font.Normal
    }

    text: "Menu Item"
    verticalAlignment: Text.AlignVCenter
  }

  Text {
    id: text_id_value
    anchors.fill: parent;
    anchors.rightMargin: 10
    font.family: "Helvetica"
    font.pixelSize: 16
    color: "blue"

    text: ""
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignRight
  }


  MouseArea {
    id: mouseArea
    anchors.fill: parent
    onClicked: container.clicked();
  }
}

