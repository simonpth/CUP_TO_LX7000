import QtQuick
import QtLocation
import QtPositioning
import QtQuick.Controls
import QtQuick.Layouts

import CUP_TO_LX7000

Item {
    id: root

    Plugin {
        id: mapPlugin
        name: "osm"

        PluginParameter { name: "osm.mapping.custom.host"; value: "http://b.tile.openstreetmap.org/"}
        PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: true }
    }

    Map {
        id: mapView
        anchors.fill: parent

        plugin: mapPlugin
        activeMapType: supportedMapTypes[supportedMapTypes.length-1]

        zoomLevel: AirportController.airportListModel.count > 0 ? 5 : 2
        center: AirportController.airportListModel.count > 0 ? AirportController.airportListModel.averagePosition() : QtPositioning.coordinate(0, 0)

        MapItemView {
            model: AirportController.airportSortFilterModel.useForMap ? AirportController.airportSortFilterModel : AirportController.airportListModel
            delegate: MapQuickItem {
                id: mapAptItem

                required property var modelData
                required property int index

                property bool homeApt : AirportController.airportSortFilterModel.homeAirport.coordinate === coordinate

                coordinate: modelData.coordinate
                // Anchor point adjusted to center the new visual marker
                anchorPoint.x: airportMarker.width / 2
                anchorPoint.y: airportMarker.height / 2

                z: homeApt ? 10 : 9

                sourceItem: Item {
                    id: airportMarker
                    // Dynamic sizing based on zoomLevel
                    property real currentSize: root.zoomLevel > 7 ? 40 : 30
                    width: currentSize
                    height: currentSize

                    // Click handlers for the entire marker area
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onReleased: (mouse) => {
                            if (mouse.button === Qt.RightButton) {
                                var point = Qt.point(mouse.x, mouse.y)
                                var mappedPoint = mapAptItem.mapToItem(root, point)
                                airportMenu.x = mappedPoint.x
                                airportMenu.y = mappedPoint.y
                                airportMenu.model = mapAptItem.modelData
                                airportMenu.airportIndex = AirportController.airportSortFilterModel.useForMap ?
                                                AirportController.airportSortFilterModel.getSourceIndexFromIndex(mapAptItem.index)
                                              : mapAptItem.index
                                airportMenu.open()
                            }
                        }
                    }

                    // Outer circle/square
                    Rectangle {
                        anchors.centerIn: parent
                        height: parent.height / 1.65
                        width: height // Makes it a square, or radius: height/2 for a circle
                        color: "transparent"
                        // Using existing color logic for home airport vs. others
                        border.color: mapAptItem.homeApt ? "red" : "green"
                        border.width: root.zoomLevel > 7 ? 4 : 2
                        radius: height / 2 // Make it a circle
                    }
                    // Inner cross/line
                    Rectangle {
                        anchors.centerIn: parent
                        height: parent.height
                        width: parent.width / 4.5
                        color: "transparent"
                        // Using existing color logic for home airport vs. others
                        border.color: mapAptItem.homeApt ? "red" : "green"
                        border.width: root.zoomLevel > 7 ? 3 : 1.5
                        rotation: mapAptItem.modelData.runwayDirection // Rotate based on runwayDirection
                    }
                }
            }
        }

        MapCircle {
            id: includedBorder
            center: AirportController.airportSortFilterModel.homeAirport.coordinate
            radius: AirportController.airportSortFilterModel.maxDistance
            color: "transparent"
            border.color: "red"
            border.width: 4
            referenceSurface: QtLocation.ReferenceSurface.Globe
            opacity: AirportController.airportSortFilterModel.useForMap ? 1 : 0
        }
    }

    PinchHandler {
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
        property var startCentroid
        onActiveChanged: {
            if (active)
                startCentroid = mapView.toCoordinate(centroid.position, false)
        }
        onScaleChanged: (delta) => {
            mapView.zoomLevel += Math.log(delta)
            mapView.alignCoordinateToPoint(startCentroid, centroid.position)
        }
    }

    WheelHandler {
        onWheel: function(event) {
            const loc = mapView.toCoordinate(point.position)
            mapView.zoomLevel += event.angleDelta.y / 120;
            mapView.alignCoordinateToPoint(loc, point.position)
        }
    }

    DragHandler {
        target: null
        grabPermissions: PointerHandler.TakeOverForbidden
        onTranslationChanged: (delta) => { mapView.pan(-delta.x, -delta.y); }
    }

    Menu {
        id: airportMenu
        property var model
        property int airportIndex

        MenuItem {
            text: "Name: " + (airportMenu.model ? airportMenu.model.name : "")
            enabled: false
        }
        MenuItem {
            text: "Code: " + (airportMenu.model ? airportMenu.model.code : "")
            enabled: false
        }
        MenuItem {
            text: "Country: " + (airportMenu.model ? airportMenu.model.country : "")
            enabled: false
        }
        MenuItem {
            text: "Elevation: " + (airportMenu.model ? airportMenu.model.elevation + " m" : "")
            enabled: false
        }

        MenuSeparator {}

        MenuItem {
            text: "Edit"
            onTriggered: {
                editDialog.airport = airportMenu.model
                editDialog.airportIndex = airportMenu.airportIndex
                editDialog.open()
            }
        }

        MenuSeparator {}

        MenuItem {
            text: "Set Home Airport"
            onTriggered: {
                AirportController.setHomeAirportByIndex(airportMenu.airportIndex)
            }
        }
    }

    AirportEditDialog {
        id: editDialog
    }
}
