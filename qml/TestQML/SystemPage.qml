// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "./Components"

Page {
    id: container

    signal dataLoadClicked;
    signal dataLoadManualClicked;
    signal secureUsbClicked;
    signal updateRepositoryClicked;
    //signal uploadResultsClicked;
    signal lanConfigClicked;
    signal wiFiConfigClicked;
    signal informationClicked;
    signal shutdownClicked;
    signal restartClicked();

    Rectangle {
        x: 40
        y: 1
        height: parent.height

        Column
        {
            HeaderText {
                text: "PDL"
            }

            GroupBox {
                id: menupdl

                radius: 10
                border.width: 2
                border.color: "gray"

                color: "lightgray"

                signal administration;
                signal information;

                MenuItem {
                    id: updateRepoItem
                    text: "Update Repository";
                    onClicked: container.updateRepositoryClicked();
                }
                MenuItem {
                    text: "Upload Results";
                    onClicked: container.uploadResultsClicked();
                }
                MenuItem {
                    text: "LAN Configuration"
                    onClicked: container.lanConfigClicked();
                    visible: false
                }
                MenuItem {
                    text: "WiFi Configuration"
                    onClicked: container.wiFiConfigClicked();
                    visible: false
                }
                MenuItem {
                    text: "Information";
                    onClicked: container.informationClicked();
                }
            }


            HeaderText {
                text: qsTr("Reset")
            }

            Button {
                id: btnShutdown
                text: qsTr("Shutdown");

                onClicked: container.shutdownClicked()
            }
            Button {
                id: btnRestart
                text: qsTr("Restart");

                onClicked: container.restartClicked()
            }
            /* Button {
          width: 400
          text: qsTr("Shutdown");
          onClicked: container.shutdownClicked()
        } */
        }
    }
}
