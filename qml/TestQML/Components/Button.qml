// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {
    id: container

    property variant text
    property alias textSize: textid.font.pointSize
    signal clicked

//    height: textid.height + 25;
    width: Styler.Sizes.Button.Width
    height: Styler.Sizes.Button.Height
    border.width: 2
    border.color: "gray"
    radius: 10
    smooth: true

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: container.enabled ?
                     (!mouseArea.pressed ? activePalette.light : activePalette.button)
                   : "lightgray"
        }
        GradientStop {
            position: 1.0
            color: container.enabled ?
                    (!mouseArea.pressed ? activePalette.button : activePalette.dark)
                   : "lightgray"
        }
    }

    SystemPalette { id: activePalette }

    /* MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked()
    }*/

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            //console.log("Button Clicked");
        }
        onPressed: {
            container.clicked()
            //console.log("Button Pressed");
        }
        onReleased: {
            //console.log("Button Released");
        }
    }
    Text {
        id: textid
        anchors.centerIn:parent
        font.pointSize: 12
        font.bold: true
        font.family: "Helvetica"
        text: parent.text
        color: activePalette.buttonText
    }
}
