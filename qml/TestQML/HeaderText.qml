import QtQuick 2.5
import "../js/Styler.js" as Styler

Item {
    width: 400
    height: 50

    property alias text: textLabel.text

    Text {
        id: textLabel
        anchors {
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
            bottomMargin: 5
        }

        font {
            family: Styler.Fonts.Family
            pixelSize: Styler.Fonts.SectionHeader.PixelSize
            weight: Styler.Fonts.SectionHeader.Weight == "Bold" ? Font.Bold : Font.Normal
        }

        width: 400
    }
}
