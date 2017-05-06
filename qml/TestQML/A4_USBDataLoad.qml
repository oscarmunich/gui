import QtQuick 2.5
import "./Components"

Page {
    id: container

    signal selectData( string name, string selected)

    Flickable {
        anchors.topMargin: 1
        anchors.fill: parent
        contentHeight: column.childrenRect.height + 20
        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            HeaderText {
                text: "USB"
            }

            Rectangle {
                radius: 10
                border.width: 2
                border.color: "blue"

                width: 400
                height: 240

                color: "lightblue"
                Column {
                    anchors.fill: parent;
                    anchors.margins: 5
                    spacing: 1
                    Row {
                        SmallValItem {
                            text: "Tail sign"
                            value: backend.tailsign
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Software"
                            value: backend.software
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Software title"
                            value: backend.softwareTitle
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Disk"
                            value: backend.li_Disk
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
                            text: "File"
                            value: backend.li_File
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Block"
                            value: backend.li_Block
                            width: 190
                        }
                        SmallValItem {
                            text: "Blocks"
                            value: backend.li_Blocks
                            width: 200
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Estimate Time Remaining"
                            value: backend.remainTime
                            width: 390
                        }
                    }
                }

            }

            HeaderText {
                text: ""
            }

            Button {
                text: backend.installRunning?"Abort Data Load":"Start Data Load"
                enabled: backend.software != ""
                onClicked: {
                    if (backend.installRunning) {
                        backend.stopDataLoad();
                    } else {
                        backend.startDataLoad();
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

