// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../../js/Styler.js" as Styler

Rectangle {

    property alias text: text_id.text
    property alias value: text_id_value.text
    property alias clickable: mouseArea.enabled
    property bool  showSelector: true
    signal clicked

    id: container
    width: Styler.Sizes.MenuItem.Width
    height: Styler.Sizes.MenuItem.Height
    //height: 30
    color: "white"
    radius: 5
    SystemPalette { id: activePalette }

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: !mouseArea.pressed ? activePalette.light : activePalette.button
        }
        GradientStop {
            position: 1.0
            color:  !mouseArea.pressed ? activePalette.button : activePalette.dark
        }
    }

    Text {
        id: text_id

        anchors {
            fill: parent
            leftMargin: 10
        }

        font {
            family: Styler.Fonts.Family
            pixelSize: Styler.Fonts.ItemValue.PixelSize
            weight: Styler.Fonts.ItemValue.Weight == "Bold" ? Font.Bold : Font.Normal
        }

        text: "Menu Item"
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: text_id_fill
        anchors.fill: parent;
        anchors.rightMargin:  10

        font {
            family: Styler.Fonts.Family
            pixelSize: Styler.Fonts.ItemValue.PixelSize
            weight: Styler.Fonts.ItemValue.Weight == "Bold" ? Font.Bold : Font.Normal
        }

        visible: container.showSelector
        text: ">"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
    }

    Text {
        id: text_id_value
        anchors.fill: parent;
        anchors.rightMargin: 30

        font {
            family: Styler.Fonts.Family
            pixelSize: Styler.Fonts.ItemValue.PixelSize
            weight: Styler.Fonts.ItemValue.Weight == "Bold" ? Font.Bold : Font.Normal
        }

        text: ""
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
    }


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked();
        //onPressed: container.clicked();
    }
}

