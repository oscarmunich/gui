// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import Models 1.0
import "./Components"

Page {
    id: container

    signal repositoryClicked;

    Flickable {
        anchors.topMargin: 1
        anchors.fill: parent
        contentHeight: column.childrenRect.height + 20
        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            HeaderText {
                text: "Counts"
            }

            Rectangle {
                radius: 10
                border.width: 2
                border.color: "blue"

                width: 400
                height: 175

                color: "lightblue"
                Column {
                    anchors.fill: parent;
                    anchors.margins: 5
                    spacing: 1
                    Row {
                        SmallValItem {
                            text: "Assigned SWPNs"
                            value: backend.repositoryAll
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Valid"
                            value: backend.repositoryValid
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Expired"
                            value: backend.repositoryExpired
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Not On PDL"
                            value: backend.repositoryNotDown
                            width: 390
                        }
                    }
                    Row {
                        SmallValItem {
                            text: "Not Referenced"
                            value: backend.repositoryNotRef
                            width: 390
                        }
                    }
                }

            }

            HeaderText {
                text: "Purge"
            }

            Button {
                text: "Purge all"
                enabled: true
                onClicked: {
                    messageBoxClearRepository.show(qsTr("Do you really want to purge repository? WARNING: After repository has been purged a complete synchronisation is needed in order to use PDL for data load."), qsTr("Yes"), qsTr("No"));
                }
            }
            Button {
                text: "Purge not referenced"
                enabled: true
                onClicked: {
                    backend.purgeRepository()
                }
            }

            HeaderText {
                text: "View"
            }

            Button {
                text: "View valid"
                enabled: true
                onClicked: {
                    container.repositoryClicked()
                }
            }
            Button {
                text: "View expired"
                enabled: true
                onClicked: {
                }
            }
            Button {
                text: "View not on PDL"
                enabled: true
                onClicked: {
                }
            }
            Button {
                text: "View not referenced"
                enabled: true
                onClicked: {
                }
            }
        }
    }
}
