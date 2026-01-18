import QtQuick
import QtQuick.Controls

import CUP_TO_LX7000

ApplicationWindow {
    id: root
    width: 1440
    height: 840
    visible: true
    title: qsTr("CUP to LX7000")

    // State management for switching between views
    property bool settingsVisible: false

    SplitView {
        anchors.fill: parent

        Item {
            SplitView.preferredWidth: 0.33 * parent.width
            SplitView.fillHeight: true
            SwipeView {
                id: leftPanelSwipe
                anchors.fill: parent
                anchors.margins: 10
                interactive: false
                currentIndex: root.settingsVisible ? 1 : 0
                clip: true

                AirportImportView {
                    id: airportImportView

                    airportList: AirportController.airportListModel

                    onProceedToSettings: {
                        root.settingsVisible = true;
                    }
                }

                SettingsView {
                    id: settingsView

                    onBackToImport: {
                        root.settingsVisible = false;
                    }
                }
            }
        }

        AirportMap {
            SplitView.fillHeight: true
        }
    }
}
