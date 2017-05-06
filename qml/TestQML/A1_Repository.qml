import QtQuick 2.5
import "./Components"
import "./Customized"

Page {
    id: root
    height: 400

    TableView {
        id: tableView

        anchors.fill: parent
    }

    onVisibleChanged: {
        if (visible)
            tableView.update();
    }
}
