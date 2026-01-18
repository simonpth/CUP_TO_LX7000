import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import CUP_TO_LX7000

Dialog {
    id: root
    title: "Edit Airport"
    modal: true
    focus: true
    standardButtons: Dialog.Save | Dialog.Cancel

    width: 640
    height: 480
    anchors.centerIn: Overlay.overlay
    popupType: Popup.Native

    property var airport
    property int airportIndex

    // Model for the style ComboBox
    property var styleModel: [
        { styleId: 0, styleName: "Unknown" },
        { styleId: 1, styleName: "Waypoint" },
        { styleId: 2, styleName: "Airfield with grass surface runway" },
        { styleId: 3, styleName: "Outlanding" },
        { styleId: 4, styleName: "Gliding airfield" },
        { styleId: 5, styleName: "Airfield with solid surface runway" },
        { styleId: 6, styleName: "Mountain Pass" },
        { styleId: 7, styleName: "Mountain Top" },
        { styleId: 8, styleName: "Transmitter Mast" },
        { styleId: 9, styleName: "VOR" },
        { styleId: 10, styleName: "NDB" },
        { styleId: 11, styleName: "Cooling Tower" },
        { styleId: 12, styleName: "Dam" },
        { styleId: 13, styleName: "Tunnel" },
        { styleId: 14, styleName: "Bridge" },
        { styleId: 15, styleName: "Power Plant" },
        { styleId: 16, styleName: "Castle" },
        { styleId: 17, styleName: "Intersection" },
        { styleId: 18, styleName: "Marker" },
        { styleId: 19, styleName: "Control/Reporting Point" },
        { styleId: 20, styleName: "PG Take Off" },
        { styleId: 21, styleName: "PG Landing Zone" }
    ]

    onAccepted: {
        AirportController.airportListModel.updateAirport(
            airportIndex,
            nameField.text,
            codeField.text,
            countryField.text,
            parseFloat(latField.text),
            parseFloat(lonField.text),
            parseFloat(elevationField.text),
            styleComboBox.currentValue,
            rwDirSpinBox.value,
            parseFloat(rwLenField.text),
            parseFloat(rwWidthField.text),
            freqField.text,
            descArea.text,
            userdataField.text
        )
    }

    onRejected: {
        root.airport = ""
    }

    Component.onCompleted: {
        if (root.airport) {
            var styleValue = root.airport.style;
            for (var i = 0; i < styleModel.length; i++) {
                if (styleModel[i].styleId === styleValue) {
                    styleComboBox.currentIndex = i;
                    break;
                }
            }
        }
    }

    ColumnLayout {
        spacing: 10
        width: parent.width
        clip: true

        GroupBox {
            title: "General"
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                columnSpacing: 10
                width: parent.width

                Label { text: "Name:" }
                TextField {
                    id: nameField
                    text: root.airport ? root.airport.name : ""
                    Layout.fillWidth: true
                }

                Label { text: "Code:" }
                TextField {
                    id: codeField
                    text: root.airport ? root.airport.code : ""
                    Layout.fillWidth: true
                }

                Label { text: "Country:" }
                TextField {
                    id: countryField
                    text: root.airport ? root.airport.country : ""
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            title: "Location & Elevation"
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                columnSpacing: 10
                width: parent.width

                Label { text: "Latitude:" }
                TextField {
                    id: latField
                    text: root.airport ? root.airport.coordinate.latitude.toFixed(6) : ""
                    validator: DoubleValidator { decimals: 6 }
                    Layout.fillWidth: true
                }

                Label { text: "Longitude:" }
                TextField {
                    id: lonField
                    text: root.airport ? root.airport.coordinate.longitude.toFixed(6) : ""
                    validator: DoubleValidator { decimals: 6 }
                    Layout.fillWidth: true
                }

                Label { text: "Elevation (m):" }
                TextField {
                    id: elevationField
                    text: root.airport ? String(root.airport.elevation) : ""
                    validator: DoubleValidator {}
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            title: "Runway"
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                columnSpacing: 10
                width: parent.width

                Label { text: "Direction:" }
                SpinBox {
                    id: rwDirSpinBox
                    from: 0
                    to: 360
                    value: root.airport ? root.airport.runwayDirection : 0
                    Layout.fillWidth: true
                }

                Label { text: "Length (m):" }
                TextField {
                    id: rwLenField
                    text: root.airport ? String(root.airport.runwayLength) : ""
                    validator: DoubleValidator {}
                    Layout.fillWidth: true
                }

                Label { text: "Width (m):" }
                TextField {
                    id: rwWidthField
                    text: root.airport ? String(root.airport.runwayWidth) : ""
                    validator: DoubleValidator {}
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            title: "Details"
            Layout.fillWidth: true
            GridLayout {
                columns: 2
                columnSpacing: 10
                width: parent.width

                Label { text: "Style:" }
                ComboBox {
                    id: styleComboBox
                    model: root.styleModel
                    textRole: "styleName"
                    valueRole: "styleId"
                    currentIndex: root.airport ? root.airport.style : 0
                    Layout.fillWidth: true
                }

                Label { text: "Frequency:" }
                TextField {
                    id: freqField
                    text: root.airport ? root.airport.frequency : ""
                    validator: RegularExpressionValidator { regularExpression: /^(?:\d{1,2}|\d{3}(?:\.\d{1,3})?)$/ }
                    Layout.fillWidth: true
                }

                Label { text: "Userdata:" }
                TextField {
                    id: userdataField
                    text: root.airport ? root.airport.userdata : ""
                    Layout.fillWidth: true
                }
            }
        }

        GroupBox {
            title: "Description"
            Layout.fillWidth: true
            TextArea {
                id: descArea
                width: parent.width
                text: root.airport ? root.airport.description : ""
            }
        }
    }
}
