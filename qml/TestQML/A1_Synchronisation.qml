// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "./Components"

Page {
    id: container

    signal selectData( string name, string selected)

    function setData(datatype, value)
    {
        if (datatype === "Connection Type")
        {
            bsConnectionType.value = value;
        }
    }

    Rectangle
    {
        x: 40
        y: 1

        Column {

            HeaderText {
                text: "Connection Type";
                visible: false
            }

            Rectangle {
                radius: 10
                border.width: 2
                border.color: "gray"

                width: 400
                height: 100
                visible: false
                color: "lightgray"
                Column {
                    anchors.fill: parent;
                    anchors.margins: 5
                    spacing: 1

                    MenuItem {
                        id: bsConnectionType
                        width: 390
                        text: "Type"
                        value: "WiFi"

                        onClicked: container.selectData("Connection Type", value);
                    }
                }
            }

            HeaderText {
                text: ""
            }

            Rectangle {
                radius: 10
                border.width: 2
                border.color: "gray"
                visible: false

                width: 400
                height: 12 + (36 * 5)

                color: "lightgray"
                Column {
                    anchors.fill: parent;
                    anchors.margins: 5
                    spacing: 1

                    ValueItem {
                        text: "Connection Type:"
                        value: backend.connectionType
                    }
                    ValueItem {
                        text: "Start Time:";
                        value: backend.updateStartTime
                    }
                    ValueItem {
                        text: "Elapsed Time:"
                        value: backend.elapsedTime
                    }
                    ValueItem {
                        text: "Progress:"
                        value: backend.updateProgress
                    }
                    ValueItem {
                        text: "Status:"
                        value: backend.updateStatus
                    }
                }
            }

            Rectangle {
                radius: 10
                border.width: 2
                border.color: "blue"

                width: 400
                height: 270

                color: "lightblue"
                Column {
                    anchors.fill: parent;
                    anchors.margins: 5
                    spacing: 1
                    Row {
                        SmallValItem {
                            text: "Connection Type:"
                            value: backend.connectionType
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Start Time:";
                            value: backend.updateStartTime
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Elapsed Time:"
                            value: backend.elapsedTime
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Total SWPNS:"
                            value: backend.li_File
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Missing:"
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "SWPNs"
                            value: backend.li_Blocks
                            width: 190
                        }
                        SmallValItem {
                            text: "Disks"
                            value: backend.li_Disks
                            width: 200
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Progress:"
                            value: backend.updateProgress
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Status:"
                            value: backend.updateStatus
                            width: 390
                        }
                    }
                }

            }

            HeaderText {
                height: 20
                text: "";
                visible: true
            }

            Button {
                text:    backend.syncButtonText
                enabled: backend.syncButtonEnabled
                onClicked: {
                         backend.syncButtonPressed();
                }
            }

            HeaderText {
                height: 20
                text: "";
                visible: true
            }

            Rectangle {
                width: 400
                height: 320
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
