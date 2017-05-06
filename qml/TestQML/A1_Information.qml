// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import "./Components"

Page {
    id: container

    signal shutdownClicked;
    signal internalClicked;

    QtObject {
        id: internal

        function stringToArray(string) {
            return string.substr(1, string.length - 2).split("#");
        }
    }

    Component.onCompleted: {
        configHelper.execute("C GetDeviceInfo");
        configHelper.execute("C GetSystemInfo");
        configHelper.execute("C GetRepositoryInfo");
        configHelper.execute("C GetEthernetInfo");
        configHelper.execute("C Get3GInfo");
        configHelper.execute("C GetWiFiInfo");
        configHelper.execute("C GetDnsInfo");
    }

    Connections {
        target: configHelper

        onReply: {
            var values;
            if (command == "GetDeviceInfo") {
                values = internal.stringToArray(response);
                if (values.length === 5) {
                    //devNameLabel.value = values[0];
                    serialNumberLabel.value = values[1];
                    macAddressLabel.value = values[2];
                    //wiFiAddressLabel.value = values[3];
                    //imeiLabel.value = values[4];
                }
            } else if (command == "GetSystemInfo") {
                values = internal.stringToArray(response);
                if (values.length === 4) {
                    capacityLabel.value = values[0];
                    availableLabel.value = values[1];
                    osLabel.value = values[2];
                    loaderVersionLabel.value = values[3];
                }
            } else if (command == "GetRepositoryInfo") {
                values = internal.stringToArray(response);
                if (values.length === 3) {
                    //lastUpdateLabel.value = values[0];
                    //swpnLabel.value = values[1];
                    //repSizeLabel.value = values[2];
                }
            } else if (command == "GetEthernetInfo") {
                values = internal.stringToArray(response);
                if (values.length === 4) {
                    dhcpLabel.value = values[0];
                    ipLabel.value = values[1];
                    subnetLabel.value = values[2];
                    //gatewayLabel.value = values[3];
                }
            } else if (command == "GetWiFiInfo") {
                values = internal.stringToArray(response);
                if (values.length === 4) {
                    wifiDhcpLabel.value = values[0];
                    wifiIpLabel.value = values[1];
                    wifiSubnetLabel.value = values[2];
                    //wifiGatewayLabel.value = values[3];
                }
            } else if (command == "Get3GInfo") {
                values = internal.stringToArray(response);
                if (values.length === 4) {
                    //wifiDhcpLabel.value = values[0];
                    threeGIpLabel.value = values[1];
                    threeGSubnetLabel.value = values[2];
                    //wifiGatewayLabel.value = values[3];
                }
            } else if (command == "GetDnsInfo") {
                values = internal.stringToArray(response);
                if (values.length === 2) {
                    primaryDnsLabel.value = values[0];
                    secondaryDnsLabel.value = values[1];
                }
            }
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: column.childrenRect.height + 20

        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter

            spacing: 10

            HeaderText {
                text: "Device Identification"
            }

            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: serialNumberLabel
                    text: "Serial Number";
                }
                ValueItem {
                    id: macAddressLabel
                    text: "MAC Adress"
                }
            }

            HeaderText {
                text: "System"
            }

            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: capacityLabel
                    text: "Capacity"
                }
                ValueItem {
                    id: availableLabel
                    text: "Available"
                }
                ValueItem {
                    id: osLabel
                    text: "Operating System"
                }
                ValueItem {
                    id: loaderVersionLabel
                    text: "Loader Ver. No"
                }
            }
            HeaderText {
                text: "Network"
            }

            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: ethernetTypeLabel
                    text: "Connection Type"
                    value: "Ethernet"
                }
                ValueItem {
                    id: dhcpLabel
                    text: "DHCP Enabled"
                }
                ValueItem {
                    id: ipLabel
                    text: "IP Address"
                }
                ValueItem {
                    id: subnetLabel
                    text: "Subnet Mask"
                }
            }

            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: wifiTypeLabel
                    text: "Connection Type"
                    value: "WiFi"
                }
                ValueItem {
                    id: wifiDhcpLabel
                    text: "DHCP Enabled"
                }
                ValueItem {
                    id: wifiIpLabel
                    text: "IP Address"
                }
                ValueItem {
                    id: wifiSubnetLabel
                    text: "Subnet Mask"
                }
            }

            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: threeGTypeLabel
                    text: "Connection Type"
                    value: "3G"
                }
                ValueItem {
                    id: threeGIpLabel
                    text: "IP Address"
                }
                ValueItem {
                    id: threeGSubnetLabel
                    text: "Subnet Mask"
                }
            }
            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: primaryDnsLabel
                    text: "Primary DNS"
                }
                ValueItem {
                    id: secondaryDnsLabel
                    text: "Secondary DNS"
                }
            }
            Button {
                text: "Connect with Remote Support"
                onClicked: backend.connRemoteSuport();
            }
            GroupBox {
                radius: 10
                border.width: 2
                border.color: "gray"

                ValueItem {
                    id: guiBuildInfo
                    text: "Build info"
                    value: backend.buildInfo
                }
            }
            Button {
                text: "Internal"
                visible: backend.isDemoPDLMode();
                onClicked: container.internalClicked();
            }
        }
    }
}

