// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "../colibri_mod"
import "./Components"

Page {
    id: container

    signal dataLoad6153Clicked
    signal dataLoad615AClicked
    signal dataLoadUSBClicked
    signal selectData( string name, string selected)
    signal tailChanged
    signal swpnChanged

    MessageBox {
        id: messageBoxAbort
        z: 100
        anchors {
            bottom: parent.bottom
        }
        onAccepted: {
            backend.stopDataLoad();
            messageBoxAbort.visible = false;
        }
        onRejected: messageBoxAbort.visible = false;
    }

    onVisibleChanged: {
        //clkeyboard1.text  = ""
        //clkeyboard2.text  = ""
    }

    Rectangle {
        x: 40
        y: 1
        color: "lightgray"
        focus: true

        Column {
            spacing: 7
            HeaderText {
                text: "Selection";
            }
            Column {
                spacing: 7
                Row { // Tailsign --------------------------------
                    width: 200
                    height: 40
                    Text {
                        id: text1
                        anchors.verticalCenter: textInput1.verticalCenter
                        height: 25
                        width: 160
                        text: qsTr("A/C Reg")
                        font.pixelSize: 22
                    }

                    TextInputBackground {
                        id: textInput1
                        width: 125
                        height: 40
                        useDefaultKeyboard: false
                        color: cvaluebase.getTailStateColor();

                        onGotFocus: {
                            cvaluebase.isTailsign = focus
                            kb1.visible = focus;
                        }

                        onFocusChanged: {
                            cvaluebase.isTailsign = focus
                            kb1.visible = focus;
                        }
                        onTextChanged: {
                            backend.doCheckTail(cvaluebase.tailsign, cvaluebase.isValid)
                            textInput1.color = cvaluebase.getTailStateColor();
                        }
                        text: cvaluebase.tailsign
                    }
                }

                Row { // SWPN     --------------------------------
                    width: 400
                    height: 40
                    Text {
                        id: text2
                        anchors.verticalCenter: textInput2.verticalCenter
                        height: 25
                        text: "Software P/N";
                        font.pixelSize: 22
                        width: 160
                    }
                    TextInputBackground {
                        id: textInput2
                        width: 200
                        height: 40

                        useDefaultKeyboard: false
                        color: cvaluebase.getSwpnStateColor();

                        onGotFocus: {
                            kb2.visible = focus;
                        }
                        onFocusChanged: {
                            kb2.visible = focus;
                        }
                        onTextChanged: {
                            backend.doCheckSwpn(cvaluebase.swpn, cvaluebase.isValid)
                            textInput2.color = cvaluebase.getSwpnStateColor();
                        }
                        text: cvaluebase.swpn
                    }
                }
            }


            Rectangle {
                height: 6
                visible: true
            }

            Row {
                width: 396
                Button {
                    id: a6153
                    text: "615-3"
                    enabled: backend.a6153load
                    visible: backend.getA6153Enabled()
                    width: parent.width / 3
                    onClicked: {
                        container.dataLoad6153Clicked()
                     }
                }
                Button {
                    id: a615a
                    text: "615A"
                    enabled: backend.a615aload
                    visible: backend.getA615AEnabled()
                    width: parent.width / 3
                    onClicked: {
                        container.dataLoad615AClicked()
                    }
                }
                Button {
                    id: usb
                    text: "USB"
                    enabled: backend.usbload
                    visible: backend.getSecureUsbEnabled()
                    width: parent.width / 3
                    onClicked: {
                        container.dataLoadUSBClicked()
                    }
                }
            }

            Rectangle {
                width: 400
                height: 190
                color: "white"
                radius: 10
                visible: backend.showLogWindow;
                border.width: 2
                border.color: "gray"

                Flickable {
                    contentHeight: logid.height
                    contentWidth: logid.width
                    anchors.margins: 10
                    anchors.fill: parent;
                    clip: true
                    flickableDirection: Flickable.VerticalFlick
                    contentY: logid.height - height;

                    Text {
                        id: logid
                        text: backend.logText;
                    }
                }
            }
        }
    }
    Rectangle {
        id: kb1
        height: 250
        color: "white"
        radius: 10
        border.width: 2
        border.color: "gray"
        anchors.bottom: container.bottom
        visible: false
        RepKeyboard {
            id: clkb1
        }
    }
    Rectangle {
        id: kb2
        height: 250
        color: "red"
        radius: 10
        border.width: 2
        border.color: "gray"
        anchors.bottom: container.bottom
        visible: false
        RepKeyboard {
            id: clkb2
        }
    }
}
