// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.5
import Models 1.0
import "../js/Styler.js" as Styler
import "./Components"

Page {
    id: container

    signal dataSelected(string name, string value);

    property string displayName;

    function loadData(name)
    {
        displayName = name;

        if (displayName == "Airline")  {
            dataLoadModel.loadAirlines();
            return;
        }
        if (displayName == "ACType") {
            displayName = "A/C Model";
            dataLoadModel.loadActmodels(backend.airline);
            return;
        }
        if (displayName == "Tailsign") {
            displayName = "A/C Reg";
            dataLoadModel.loadTailsigns(backend.airline, backend.ACType);
            return;
        }
        if (displayName == "ATA Chapter") {
            displayName = "ATA Chapter";
            dataLoadModel.loadAtaChapters(backend.airline, backend.ACType, backend.tailsign);
            return;
        }
        if (displayName == "ATA Section") {
            displayName = "ATA Section";
            dataLoadModel.loadAtaSections(backend.airline, backend.ACType, backend.tailsign, backend.ataChapter);
            return;
        }
        if (displayName == "Software") {
            displayName = "Software P/N";
            dataLoadModel.loadSwpn(backend.airline, backend.ACType, backend.tailsign, backend.ataChapter, backend.ataSection);
            return;
        }
    }

    Rectangle {
        id: container_

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: container_.height < container.height ? undefined : parent.verticalCenter
            top: container_.height < container.height ? container.top : undefined
            topMargin: container_.height < container.height ? 10 : 0
        }

        height: {
            var h = ((list_view1.count) * Styler.Sizes.MenuItem.Height) + 10;
            return (h > container.height) ? container.height - 20 : h;
        }
        width: Styler.Sizes.MenuItem.Width + 10


        radius: 10
        border.width: 2
        border.color: "gray"
        color: "lightgray"

        ListView {
            anchors.fill: parent
            anchors.margins: 5
            clip: true
            id: list_view1

            model: DataLoadModel {
                id: dataLoadModel
            }

            delegate:
                MenuItem {
                text: displayName == "Connection Type" ? model.display : model.value
                showSelector: false
                onClicked: {
                    if (displayName == "Connection Type")
                        container.dataSelected(model.display, "");
                    else {
                        console.log("Selected item: " + displayName + " " + model.id + " " + model.value);
                        container.dataSelected(model.id, model.value);
                    }
                }
            }
        }
    }
}
