// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import Models 1.0
import "./Components"
import "../js/Styler.js" as Styler

Page {
    id: container

    signal dataLoad6153Clicked;
    signal dataLoad615AClicked;
    signal dataLoadUSBClicked;

    property string airlineText: ""

    signal selectData( string name, string selected)

    function setData(datatype, value) {
        backend.setData(datatype, value);
    }

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

    Flickable {
        anchors.topMargin: 1
        anchors.fill: parent
        contentHeight: column.childrenRect.height + 20
        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            HeaderText {
                text: "Select Software"
            }
            GroupBox {
                radius: 10
                border.width: 2
                border.color: "blue"

                color: "lightblue"

                MenuItem {
                    id: bsAirline
                    width: 390
                    text: "Airline"
                    value: airlineText;
                    enabled: !backend.installRunning

                    onClicked: container.selectData("Airline", value);
                }

                MenuItem {
                    id: bsACType
                    width: 390
                    text: "A/C Model"
                    value: backend.ACType;
                    enabled: !backend.installRunning
                    showSelector: backend.airline != ""
                    clickable: backend.airline != ""

                    onClicked: container.selectData("ACType", value);
                }

                MenuItem {
                    id: bsTailsign
                    width: 390
                    text: "A/C Reg"
                    value: backend.tailsign
                    enabled: !backend.installRunning
                    showSelector: backend.ACType != ""
                    clickable: backend.ACType != ""

                    onClicked: container.selectData("Tailsign", value);
                }

                MenuItem {
                    id: bsAtaChapter;
                    width: 390
                    text: "ATA Chapter";
                    value: backend.ataChapter
                    enabled: !backend.installRunning
                    showSelector: backend.tailsign != ""
                    clickable: backend.tailsign != ""

                    onClicked: container.selectData("ATA Chapter", value);
                }

                MenuItem {
                    id: bsAtaSection;
                    width: 390
                    text: "ATA Section";
                    value: backend.ataSection
                    enabled: !backend.installRunning
                    showSelector: backend.ataChapter != ""
                    clickable: backend.ataChapter != ""

                    onClicked: container.selectData("ATA Section", value);
                }

                MenuItem {
                    id: bsSoftware;
                    width: 390
                    text: "Software P/N";
                    value: backend.software
                    enabled: !backend.installRunning
                    showSelector: backend.ataSection != ""
                    clickable: backend.ataSection != ""

                    onClicked: container.selectData("Software", value);
                }
            }

            HeaderText {
                text: "Select Load Type"
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
}
