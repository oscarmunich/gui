// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "./Components"

Page {
    id: container

    signal dataLoadClicked;
    signal updateRepositoryClicked;
    signal informationClicked;
    signal repositoryClicked;
    signal swpnMenuSelection;
    signal swpnManualSelection;

    GroupBox {
        id: menupdl

        anchors {
            top: parent.top
            topMargin: 30
            horizontalCenter: parent.horizontalCenter
        }

        radius: 10
        border.width: 2
        border.color: "gray"

        color: "lightgray"

        signal administration;
        signal information;

        MenuItem {
            id: dataLoadItem
            text: "Data Load Home"
            onClicked: container.dataLoadClicked();
            visible: false
        }

        MenuItem {
            text: "Data Load Menu Selection"
            onClicked: container.swpnMenuSelection()
        }
        MenuItem {
            text: "Data Load Manual Selection"
            onClicked: container.swpnManualSelection()
        }

        MenuItem {
            id: syncItem
            text: "Synchronisation"
            onClicked: container.updateRepositoryClicked();
        }

        MenuItem {
            id: repositoryItem
            text: "Repository"
            onClicked: container.repositoryClicked();
        }
        MenuItem {
            text: "System Information"
            onClicked: container.informationClicked();
        }
    }

    Column {
        anchors {
            bottom: parent.bottom
            bottomMargin: 15
            horizontalCenter: parent.horizontalCenter
        }

        ValueItem {
            text: qsTr("PDL-Name:")
            value: backend.pdlName
            color: "transparent"
            gradient: null
        }

        ValueItem {
            text: qsTr("Last sync:")
            value: backend.lastSync
            color: "transparent"
            gradient: null
        }

    }
}
