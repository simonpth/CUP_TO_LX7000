import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import CUP_TO_LX7000

ColumnLayout {
    id: root

    required property AirportListModel airportList

    signal proceedToSettings()

    Label {
        text: "Import CUP Files"
        font.pixelSize: 18
        font.bold: true
        Layout.fillWidth: true
    }

    // Drop area for drag-and-drop functionality
    DropArea {
        id: dropArea
        Layout.fillWidth: true
        Layout.fillHeight: true

        onDropped: (drop) => {
            if (drop.hasUrls) {
                const urls = drop.urls;
                for (let i = 0; i < urls.length; i++) {
                    const filePath = urls[i].toString();
                    if (filePath.endsWith(".cup")) {
                        fileModel.append({ filePath: filePath });
                    } else {
                        console.log("Rejected file: " + filePath);
                    }
                }
            }
        }

        ListView {
            id: fileListView

            model: fileModel

            anchors.fill: parent

            spacing: 4

            clip: true

            delegate: Label {
                width: fileListView.width
                text: model.filePath.replace("file://", "")
                font.pixelSize: 12
                wrapMode: Text.Wrap

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: {
                        contextMenu.popup()
                    }
                }

                Menu {
                    id: contextMenu
                    MenuItem {
                        text: "Delete"
                        onTriggered: {
                            fileModel.remove(index)
                        }
                    }
                }
            }

            footer: RowLayout {
                id: footerLayout

                width: fileListView.width

                Button {
                    id: addFileButton
                    text: "+"
                    onClicked: {
                        fileDialog.open();
                    }
                    Layout.fillWidth: true
                }

                Button {
                    id: clearFilesButton
                    text: "Clear All"
                    onClicked: {
                        fileModel.clear();
                        clearAirportsButton.click()
                    }
                    Layout.fillWidth: true
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select Files"
        fileMode: FileDialog.OpenFiles
        nameFilters: ["*.cup"] // Only allow .cup files
        onAccepted: {
            for (let i = 0; i < fileDialog.selectedFiles.length; i++) {
                const filePath = fileDialog.selectedFiles[i].toString();
                if (filePath.endsWith(".cup")) {
                    fileModel.append({ filePath: filePath });
                } else {
                    console.log("Rejected file: " + filePath);
                }
            }
        }
    }

    // Import button
    Button {
        id: importButton
        text: "Import Files"
        Layout.alignment: Qt.AlignCenter
        onClicked: {
            for (let i = 0; i < fileModel.count; i++) {
                const filePath = fileModel.get(i).filePath;
                airportList.readCupFile(filePath.replace("file://", ""));
            }
        }
        Layout.fillWidth: true
    }

    // Clear airports button
    Button {
        id: clearAirportsButton

        text: "Clear Airports"
        Layout.alignment: Qt.AlignCenter
        onClicked: {
            airportList.clearAirports();
            AirportController.airportSortFilterModel.clearHomeAirport();
        }
        Layout.fillWidth: true
    }

    // Proceed to Settings button (visible only when airports are imported)
    Button {
        id: proceedButton
        text: "Continue to Settings →"
        Layout.alignment: Qt.AlignBottom | Qt.AlignRight
        visible: airportList.count > 0
        onClicked: {
            root.proceedToSettings();
        }
    }

    // File model
    ListModel {
        id: fileModel
    }
}
