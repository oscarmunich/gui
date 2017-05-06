import QtQuick 2.5

Item {
    id: root

    property int firstColumnWidth:  (root.width / 100) * 30;
    property int secondColumnWidth:  (root.width / 100) * 40;
    property int thirdColumnWidth: (root.width / 100) * 15;
    property int fourthColumnWidth: (root.width / 100) * 15;

    property variant list

    function update() {
        tableEmulator.model = backend.getRepositoryData();
    }

    Component {
        id: delegateComponent

        Row {
            id: fieldsRow

            property variant valuesList: null

            TableField {
                width: root.firstColumnWidth

                text: fieldsRow.valuesList[0]
            }
            TableField {
                width: root.secondColumnWidth

                text: fieldsRow.valuesList[1]
            }
            TableField {
                width: root.thirdColumnWidth

                text: fieldsRow.valuesList[2]
            }
            TableField {
                width: root.fourthColumnWidth

                text: fieldsRow.valuesList[3]
            }

            Component.onCompleted: {
                fieldsRow.valuesList = modelData.split(";");
            }
        }
    }


    Row {
        id: headersRow

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        TableHeader {
            width: root.firstColumnWidth
            text: "SWPN"
        }
        TableHeader {
            width: root.secondColumnWidth
            text: "Title"
        }
        TableHeader {
            width: root.thirdColumnWidth
            text: "LT"
        }
        TableHeader {
            width: root.fourthColumnWidth
            text: "MC"
        }
    }

    ListView {
        id: tableEmulator

        anchors {
            top: headersRow.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        clip: true

        delegate: delegateComponent
    }
}
