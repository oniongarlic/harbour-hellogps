import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page
    allowedOrientations: Orientation.Portrait | Orientation.Landscape

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("About");
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingMedium
            PageHeader {
                title: qsTr("Hello GPS!")
            }

            DetailItem {
                label: "GPS Time"
                value: root.gps.ts>0 ? Qt.formatDateTime(new Date(root.gps.ts*1000),"dd.MM.yyyy - hh:mm:ss" ) : "-"
            }

            DetailItem {
                label: "Satellites";
                value: root.gps.satellitesInUse+"/"+root.gps.satellitesInView
            }

            DetailItem {
                label: "Valid fix";
                value: root.gps.valid ? "Yes" : "No"
            }

            DetailItem {
                id: posLat
                label: "Latitude"
            }

            DetailItem {
                id: posLon
                label: "Longitude"
            }

            Connections {
                target: root.gps
                onPositionChanged: {
                    posLat.value=root.gps.getLatitude();
                    posLon.value=root.gps.getLongitude();
                }
            }

            /*
            DetailItem {
                label: "Accuracy";
                value: root.gps.valid ? root.gps.accuracy : "-"
            }
            */

            Button {
                text: "Goto map"
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: root.gps.valid
                onClicked: {
                    var lat=root.gps.getLatitude();
                    var lon=root.gps.getLongitude();
                    var url="http://www.openstreetmap.org/#map=16/"+lat+"/"+lon;
                    console.debug("Opening URL: "+url)
                    Qt.openUrlExternally(url);
                }
            }

            DetailItem {
                label: "Altitude";
                value: root.gps.valid ? root.gps.altitude : "-"
            }

            DetailItem {
                label: "Velocity"
                value: root.gps.valid ? root.gps.velocity : '-'
            }

            DetailItem {
                label: "Direction"
                value: root.gps.valid ? root.gps.direction : '-'
            }

            DetailItem {
                label: "Climb"
                value: root.gps.valid ? root.gps.climb : '-'
            }
        }
    }
}


