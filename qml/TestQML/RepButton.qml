import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: repbutton
    property alias text: button.text
    property int bwidth: 48
    property int bheight: 60
    property int index: 0
    property bool isenabled: false
    property string envar: cvaluebase.isEnabled
    property string envarin: envar
    property real buttonWidth: bwidth

    property Image backspaceImage : Image {
        source: "backspace_50x70.png"
        scale: buttonWidth*0.01; smooth: true
    }
    property Image nullImage: Image { //this property is "private" don't write it to documentation
        id: "null"
        source: ""
        width: repbutton.height
        height: repbutton.height
        fillMode: Image.PreserveAspectCrop
        smooth: false
        scale: 1
    }
    property Image specialImage : nullImage
    width: bwidth
    height: bheight
    Button {
        id: button
        text: '0'
        style: ButtonStyle {
            label: Text {
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Tahoma"
                font.pointSize: 24
                color: enabled ? "blue" : "grey"
                text: control.text
            }
            background: Rectangle {
                border.color: enabled ? "blue" : "grey"
                border.width: 3
                color: "lightblue"
                radius: 15
            }
        }

        Image {
            source: specialImage.source
            width: if(source != "") specialImage.width
            height: if(source != "") specialImage.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            smooth: specialImage.smooth
            scale: specialImage.scale
        }
        enabled: isenabled || repbutton.envarin & (1 << repbutton.index)
        width: repbutton.bwidth - 5
        height: repbutton.bheight - 5
        onClicked: {
            cvaluebase.keyPressed(text)
        }
    }
}
