import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {
    id: root

    property alias text: textLabel.text
    property alias fontSize: textLabel.font.pixelSize
    property alias fontBold: textLabel.font.bold
    property alias fontColor: textLabel.color

    border {
        width: 1
        color: "black"
    }

    height: 30

    Text {
        id: textLabel
        anchors.fill: parent

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: Styler.Fonts.TableField.Color

        font {
            pixelSize: Styler.Fonts.TableField.PixelSize
            bold: Styler.Fonts.TableField.Weight == "Bold"
        }
    }
}
