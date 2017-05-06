import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {
    id: checkbox

    property string text: 'Button'
    property RadioGroup radioGroup
    signal checkedRadioButton()

    color: "transparent"
    Row{
        spacing:10
        width: checkbox.width
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            width: 20
            height: checkBoxText.height
            color: "transparent"
            Image {
                id: imgCheckBox
                anchors.centerIn: parent
                source: radioGroup.selected === checkbox ? "../../../resources/bullet-s.png" : "../../../resources/bullet-p.png"
            }
        }
        Text {
            id: checkBoxText
            text: checkbox.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font {
                family: Styler.Fonts.Family
                pixelSize: Styler.Fonts.ItemValue.PixelSize
                weight: Styler.Fonts.ItemValue.Weight == "Bold" ? Font.Bold : Font.Normal
            }
        }
    }

    MouseArea {
        id: sideButtonMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            checkbox.radioGroup.selected = checkbox
            checkbox.checkedRadioButton()
        }
    }
}
