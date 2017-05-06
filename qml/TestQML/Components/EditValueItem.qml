import QtQuick 2.5
import "../"
import "../../js/Styler.js" as Styler

Rectangle {
    id: root

    property alias text: labelText.text
    property alias value: textInput.text
    property alias valueMask: textInput.inputMask
    property alias validator: textInput.validator
    property alias echoMode: textInput.echoMode
    property alias numericValue: textInput.numeric

    width: 390
    height: 35

    color: "white"
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

    Row {
        anchors {
            fill: parent
            leftMargin: 10
        }
        spacing: 20

        Text {
            id: labelText

            width: 190
            height: 30

            font {
                family: Styler.Fonts.Family
                pixelSize: Styler.Fonts.ItemTitle.PixelSize
                weight: Styler.Fonts.ItemTitle.Weight == "Bold" ? Font.Bold : Font.Normal
            }

            verticalAlignment: Text.AlignVCenter
        }

        TextInputBackground {
            id: textInput

            width: 165
            height: 30
        }
    }
}
