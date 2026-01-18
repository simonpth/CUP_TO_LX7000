import QtQuick
import QtQuick.Controls
import QtQml.Models

import CUP_TO_LX7000

Item {
    id: root

    signal airportSelected(int index)

    implicitHeight: airportSearch.implicitHeight
    implicitWidth: airportSearch.implicitContentWidth

    SortFilterProxyModel {
        id: airportFilterModel

        model: AirportController.airportListModel

        filters: [
            FunctionFilter {
                id: functionFilter
                component RoleData: QtObject {
                    property string name
                }
                function filter(data: RoleData) : bool {
                    return airportSearch.text !== "" && data.name.toLowerCase().startsWith(airportSearch.text.toLowerCase());
                }
            }
        ]
    }

    SearchField {
        id: airportSearch

        anchors.fill: parent

        suggestionModel: airportFilterModel
        textRole: "name"

        live: true
        onSearchTriggered: airportFilterModel.invalidate()
        popup.height: Math.min(popup.contentHeight, 300)

        onActivated: (index) => {
                         airportSearch.focus = false
                         root.airportSelected(airportFilterModel.mapToSource(airportFilterModel.index(index, 0)).row)
                     }
    }
}
