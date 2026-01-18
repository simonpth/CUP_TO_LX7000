import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.synchronizer
import QtQuick.Dialogs
import QtCore

import CUP_TO_LX7000

ColumnLayout {
    id: root

    signal backToImport()

    // Header
    Label {
        text: "Export Settings"
        font.pixelSize: 18
        font.bold: true
        Layout.fillWidth: true
    }

    Label {
        text: "Select Home Airport"
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignBottom
    }

    AirportSearchView{
        id: airportSearchComboBox

        Layout.fillWidth: true

        onAirportSelected: (index) => {
                               AirportController.setHomeAirportByIndex(index)
                               //update distance if page is maxAptPage
                               if(distanceSlection.currentIndex === 1) {
                                   var distance = AirportController.airportSortFilterModel.distanceForAirportAmount(maxAptSlider.value);
                                   AirportController.airportSortFilterModel.maxDistance = distance;
                               }
                           }
    }

    SwipeView {
        id: distanceSlection
        Layout.fillWidth: true
        Layout.preferredHeight: 150
        clip: true

        Page {
            id: maxDistancePage
            title: "Max Distance"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "Maximum distance from home airport (km)"
                    Layout.fillWidth: true
                }

                Slider {
                    id: maxDistanceSlider
                    from: 0
                    to: 900
                    value: 200
                    stepSize: 1
                    Layout.fillWidth: true

                    onValueChanged: {
                        AirportController.airportSortFilterModel.maxDistance = value * 1000
                    }

                    Component.onCompleted: AirportController.airportSortFilterModel.maxDistance = value * 1000
                }

                TextField {
                    id: maxDistanceTextField
                    validator: IntValidator { bottom: 0; top: 900 }
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Synchronizer {
                    sourceObject: maxDistanceSlider
                    sourceProperty: "value"
                    targetObject: maxDistanceTextField
                    targetProperty: "text"
                }
            }
        }

        Page {
            id: maxAptPage
            title: "Max Airports"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "Maximum number of airports to export"
                    Layout.fillWidth: true
                }

                Slider {
                    id: maxAptSlider
                    from: 1
                    to: 5000
                    value: 100
                    stepSize: 1
                    Layout.fillWidth: true

                    onValueChanged: {
                        var distance = AirportController.airportSortFilterModel.distanceForAirportAmount(maxAptSlider.value);
                        AirportController.airportSortFilterModel.maxDistance = distance;
                    }
                }

                TextField {
                    id: maxAptTextField
                    validator: IntValidator { bottom: 1; top: 5000 }
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Synchronizer {
                    sourceObject: maxAptSlider
                    sourceProperty: "value"
                    targetObject: maxAptTextField
                    targetProperty: "text"
                }
            }
        }

        onCurrentIndexChanged: {
            if(currentIndex === 0) {
                AirportController.airportSortFilterModel.maxDistance = maxDistanceSlider.value
            } else {
                var distance = AirportController.airportSortFilterModel.distanceForAirportAmount(maxAptSlider.value);
                AirportController.airportSortFilterModel.maxDistance = distance;
            }
        }
    }

    PageIndicator {
        id: indicator
        count: distanceSlection.count
        currentIndex: distanceSlection.currentIndex
        Layout.alignment: Qt.AlignHCenter
    }

    Layout.fillWidth: true
    Label {
        text: "Waypoint Styles"
        font.pixelSize: 16
        font.bold: true
        Layout.fillWidth: true
    }

    ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        model: ListModel {
            ListElement { styleId: 0; styleName: "Unknown" }
            ListElement { styleId: 1; styleName: "Waypoint" }
            ListElement { styleId: 2; styleName: "Airfield with grass surface runway" }
            ListElement { styleId: 3; styleName: "Outlanding" }
            ListElement { styleId: 4; styleName: "Gliding airfield" }
            ListElement { styleId: 5; styleName: "Airfield with solid surface runway" }
            ListElement { styleId: 6; styleName: "Mountain Pass" }
            ListElement { styleId: 7; styleName: "Mountain Top" }
            ListElement { styleId: 8; styleName: "Transmitter Mast" }
            ListElement { styleId: 9; styleName: "VOR" }
            ListElement { styleId: 10; styleName: "NDB" }
            ListElement { styleId: 11; styleName: "Cooling Tower" }
            ListElement { styleId: 12; styleName: "Dam" }
            ListElement { styleId: 13; styleName: "Tunnel" }
            ListElement { styleId: 14; styleName: "Bridge" }
            ListElement { styleId: 15; styleName: "Power Plant" }
            ListElement { styleId: 16; styleName: "Castle" }
            ListElement { styleId: 17; styleName: "Intersection" }
            ListElement { styleId: 18; styleName: "Marker" }
            ListElement { styleId: 19; styleName: "Control/Reporting Point" }
            ListElement { styleId: 20; styleName: "PG Take Off" }
            ListElement { styleId: 21; styleName: "PG Landing Zone" }
        }
        delegate: SwitchDelegate {
            id: delegate
            text: styleName

            clip: true

            Connections {
                target: AirportController.airportListModel

                function onPossibleStylesChanged() {
                    delegate.height = AirportController.airportListModel.isStylePossible(styleId) ? delegate.implicitHeight : 0
                }
            }

            checked: AirportController.airportSortFilterModel.isStyleAllowed(styleId)
            onCheckedChanged: {
                if (checked) {
                    AirportController.airportSortFilterModel.addAllowedStyle(styleId)
                } else {
                    AirportController.airportSortFilterModel.removeAllowedStyle(styleId)
                }

                //update distance if page is maxAptPage
                if(distanceSlection.currentIndex === 1) {
                    var distance = AirportController.airportSortFilterModel.distanceForAirportAmount(maxAptSlider.value);
                    AirportController.airportSortFilterModel.maxDistance = distance;
                }
            }
        }
    }

    Switch {
        id: sortFilterModelForMapSwitch
        text: "Use Filter for Map"

        Synchronizer {
            sourceObject: AirportController.airportSortFilterModel
            sourceProperty: "useForMap"
            targetObject: sortFilterModelForMapSwitch
            targetProperty: "checked"
        }
    }

    // Back button
    RowLayout {
        Layout.alignment: Qt.AlignBottom
        Layout.fillWidth: true

        Button {
            id: backButton
            text: "← Back to Import"
            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
            onClicked: {
                root.backToImport();
            }
        }

        Item {Layout.fillWidth: true}

        Button {
            id: exportButton
            text: "Export"
            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
            onClicked: {
                fileDialog.open();
            }
        }

        FileDialog {
            id: fileDialog
            title: "Select Export File"
            defaultSuffix: "txt"
            currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            nameFilters: ["LXe (*.txt)"]
            fileMode: FileDialog.SaveFile
            onAccepted: {
                AirportController.exportAirports(selectedFile)

                var endsWithTxt = /txt$/;
                if(!endsWithTxt.test(selectedFile)) {
                    errorDialog.open();
                } else {
                    infoDialog.open();
                }
            }
        }

        MessageDialog {
            id: infoDialog
            title: "Export Successful"
            text: "Airports have been successfully exported."
            buttons: MessageDialog.Ok
        }

        MessageDialog {
            id: errorDialog
            title: "Export Failed"
            text: "Please choose a file that ends with .txt"
            buttons: MessageDialog.Ok
        }
    }
}
