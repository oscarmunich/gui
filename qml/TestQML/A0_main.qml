// import QtQuick 1.0 // to target S60 5th Edition or Memo 5
import QtQuick 2.5
import Network 1.0
import "../colibri_mod"
import "./Components"
import "../js/Styler.js" as Styler

Rectangle {
    id: main
    width: 480
    height: 800
    x: 0
    y: 0

    property string oldState: ""

    Connections {
        target: backend

        onScreenStateChanged: {
            if (!turnedOn)
                fakeTouchPage.visible = true;
            else
                fakeTouchPage.visible = false;
        }
    }


    A1_Home {
        id: homePage

        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        onDataLoadClicked: {                 
            main.state = "DataLoadPage";      
        }                              
        onUpdateRepositoryClicked: {
            backend.resetAll();
            main.oldState = main.state;
            main.state = "UpdateRepository";
        }
        onRepositoryClicked: {
            main.state = "RepositoryPage";
        }
        onInformationClicked: {
            main.oldState = main.state;
            main.state = "Information";
        }
        onSwpnMenuSelection: {
            backend.resetAll();
            screen_dataload_1.airlineText = "";
            main.state = "DataLoadMenu";
        }
        onSwpnManualSelection: {
            backend.resetAll();
            main.state = "DataLoadMan";
        }
    }

    A1_Information {
        id: screen_info

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0
        visible: false

        onInternalClicked: {
            backend.clearAuto();
            main.state = "InternalPage";
        }
    }

    A1_Repository {
        id: repositoryPage

        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0
    }

    A1_Synchronisation {
        id: screen_updatefirmware;
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        property string currentType;

        onSelectData: {
            console.log("Test:" + name + " select: " + selected);

            currentType = name;
            screen_dataselect.loadData(name);

            main.oldState = main.state;
            main.state = "DataSelect";
        }

        function test(name)
        {
            console.log("all is wel: " + name);
            setData(currentType, name);
        }
    }

    A2_DataLoadPage {
        id: dataLoadPage

        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        onSwpnMenuSelection: {
            backend.resetAll();
            screen_dataload_1.airlineText = "";
            main.state = "DataLoadMenu";
        }

        onSwpnManualSelection: {
            backend.resetAll();
            main.state = "DataLoadMan";
        }
    }

    A3_ScreenDataLoadMan {
        id: screen_dataloadman
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        onDataLoad6153Clicked: {
            main.state = "A6153DataLoadPage";
        }
        onDataLoad615AClicked: {
            main.state = "A615ADataLoadPage";
        }
        onDataLoadUSBClicked: {
            main.state = "DataLoadUsb";
        }
        property string currentType;

        onSelectData: {
            currentType = name;
            screen_dataselect.loadData(name);

            main.oldState = main.state;
            main.state = "DataSelect";
        }

        function test(name)
        {
            console.log("ScreenDataLoadMan:" + currentType + " " + name);
            setData(currentType, name);
        }
    }

    A3_ScreenDataLoadMenu {
        id: screen_dataload_1
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        onDataLoad6153Clicked: {
            main.state = "A6153DataLoadPage";
        }
        onDataLoad615AClicked: {
            main.state = "A615ADataLoadPage";
        }
        onDataLoadUSBClicked: {
            main.state = "DataLoadUsb";
        }
        property string currentType;

        onSelectData: {
            console.log("ScreenDataLoadMenu:  onSelectData:" + name + "-" + selected + " state:" + main.state);
            currentType = name;
            screen_dataselect.loadData(name);

            main.oldState = main.state;
            main.state = "DataSelect";
        }

        function test(name, value)
        {
            console.log("ScreenDataLoadMenu:" + name + " " + value);
            if (currentType == "Airline") {
                screen_dataload_1.airlineText = value;
            }
            setData(currentType, name);
        }
    }

    A3_ScreenDataSelect {
        id: screen_dataselect;
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        onDataSelected: {
            console.log("Selected: " + name + " " + main.oldState);

            if (main.oldState == "DataLoad") {
                screen_dataload.test(name);
            }
            else if (main.oldState == "DataLoadMan") {
                screen_dataloadMan.test(name);
            }
            else if (main.oldState == "DataLoadMenu") {
                screen_dataload_1.test(name, value);
            }
            else if (main.oldState == "SecureUsb") {
                screen_secureUSb.test(name);
            }
            else if (main.oldState == "UpdateRepository") {
                screen_updatefirmware.test(name)
            }
            else if (main.oldState == "Administration") {
                screen_administration.test(name)
            }
            else if (main.oldState == "DataLoadUsb") {
                dataLoadUsbPage.test(name, value);
            }

            main.state = main.oldState;
        }
    }

    A4_A6153DataLoad {
        id: a6153DataLoadPage

        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0
    }

    A4_A615ADataLoad {
        id: a615ADataLoadPage

        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0
    }

    A4_USBDataLoad {
        id: dataLoadUsbPage
        visible: false
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0

        property string currentType;

        onSelectData: {
            currentType = name;
            screen_dataselect.loadData(name);

            main.oldState = main.state;
            main.state = "DataSelect";
        }

        function test(name, value)
        {
            if (currentType == "Airline") {
                dataLoadUsbPage.airlineText = value;
            }
            setData(currentType, name);
        }
    }

    FakeTouchPage {
        id: fakeTouchPage

        anchors.fill: parent

        z: 1000

        visible: false

        onClicked: fakeTouchPage.visible = false
    }

    Rectangle {
        id: header
        width: 480
        height: 50
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#646464"
            }

            GradientStop {
                position: 0.340
                color: "#000000"
            }

            GradientStop {
                position: 0.750
                color: "#000000"
            }

            GradientStop {
                position: 1
                color: "#646363"
            }
        }
        transformOrigin: Item.Center
        visible: true
        border.color: "#000000"
        border.width: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Text {
            id: header_name
            color: Styler.Fonts.Header.Color
            text: qsTr("Home")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent

            font {
                family: Styler.Fonts.Family
                weight: Styler.Fonts.Header.Weight == "Bold" ? Font.Bold : Font.Normal
                pixelSize: Styler.Fonts.Header.PixelSize
            }
        }

        Button {
            id: header_home
            y: 12
            width: 94
            height: 20
            color: "#424891"
            radius: 15
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 10
            border.color: "#808080"
            text: qsTr("Home");
            textSize: 8
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            border.width: 2

            onClicked: {
                if (backend.homeEnabled) {
                    main.state = "Home";
                }
            }
        }
        Button {
            id: header_back
            y: 12
            width: 94
            height: 20
            color: "#424891"
            radius: 15
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 10
            border.color: "#808080"
            text: qsTr("Back");
            textSize: 8
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            border.width: 2

            visible: false
            onClicked: {
                if (backend.homeEnabled) {
                    main.state = main.oldState;
                }
            }
        }

        Button {
            id: header_purge
            y: 12
            width: 94
            height: 20
            color: "#424891"
            radius: 15
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 10
            border.color: "#808080"
            text: qsTr("Purge");
            textSize: 8
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            border.width: 2
            visible: false
            onClicked: {
                messageBoxClearRepository.show(qsTr("Do you really want to purge repository? WARNING: After repository has been purged a complete synchronisation is needed in order to use PDL for data load."), qsTr("Yes"), qsTr("No"));
            }
        }

    }

    MessageBox {
        id: messageBoxClearRepository
        z: 100
        anchors {
            bottom: parent.bottom
        }
        onAccepted: {
            backend.purgeRepository()
            repositoryPage.visible = false;
            repositoryPage.visible = true;
            messageBoxClearRepository.visible = false;
        }
        onRejected: messageBoxClearRepository.visible = false;
    }



    state: "Home"

    states: [
        State {
            name: "Home"
            PropertyChanges { target: homePage; visible: true }
            PropertyChanges { target: header_home; visible: false }
            PropertyChanges { target: header_name; text: "Start Screen" }
        },

        State { // Bypassed as Home goes direct to Menu / Manual selection
            name: "DataLoadPage"
            PropertyChanges { target: dataLoadPage; visible: true }
            PropertyChanges { target: header_name; text: "Data Load Page" }
        },

        State {
            name: "DataLoadMenu"
            PropertyChanges { target: screen_dataload_1; visible: true }
            PropertyChanges { target: header_name; text: "Menu" }
        },
        State {
            name: "DataLoadMan"
            PropertyChanges { target: screen_dataloadman; visible: true }
            PropertyChanges { target: header_name; text: "Manual" }
        },
        State {
            name: "DataSelect"
            PropertyChanges { target: screen_dataselect; visible: true }
            PropertyChanges { target: header_back; visible: true }
            PropertyChanges { target: header_name; text: screen_dataselect.displayName }
        },

        // Actual load pages for the different methods
        State {
            name: "A6153DataLoadPage"
            PropertyChanges { target: a6153DataLoadPage; visible: true }
            PropertyChanges { target: header_name; text: "ARINC 6153" }
        },
        State {
            name: "A615ADataLoadPage"
            PropertyChanges { target: a615ADataLoadPage; visible: true }
            PropertyChanges { target: header_name; text: "ARINC 615A" }
        },
        State {
            name: "DataLoadUsb"
            PropertyChanges { target: dataLoadUsbPage; visible: true }
            PropertyChanges { target: header_name; text: "USB" }
        },

        // Rest
        State {
            name: "UpdateRepository"
            PropertyChanges { target: screen_updatefirmware; visible: true }
            PropertyChanges { target: header_back; visible: true }
            PropertyChanges { target: header_name; text: "Synchronisation" }
        },
        State {
            name: "RepositoryPage"
            PropertyChanges { target: repositoryPage; visible: true }
            PropertyChanges { target: header_name; text: "Repository" }
            PropertyChanges { target: header_purge; visible: true }
        },
        State {
            name: "Information"
            PropertyChanges { target: screen_info; visible: true }
            PropertyChanges { target: header_back; visible: true }
            PropertyChanges { target: header_name; text: "Information" }
        }

    ]

}
