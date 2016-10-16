import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait | Orientation.Landscape

    readonly property string versionStr: appVersion;

    readonly property string helpText: qsTr("Shows GPS information");

    readonly property string license: 'This program is free software: you can redistribute it and/or modify ' +
                             'it under the terms of the GNU General Public License as published by ' +
                             'the Free Software Foundation, either version 2 of the License, or ' +
                             '(at your option) any later version.<br /><br />' +

                             'This package is distributed in the hope that it will be useful, ' +
                             'but WITHOUT ANY WARRANTY; without even the implied warranty of ' +
                             'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ' +
                             'GNU General Public License for more details.<br /><br />' +

                             'You should have received a copy of the GNU General Public License ' +
                             'along with this program. If not, see <a href="http://www.gnu.org/licenses">http://www.gnu.org/licenses</a><br />'

    readonly property string homePageUrl: "http://www.tal.org/projects/hello-gps"
    readonly property string twitterUrl: "https://twitter.com/oniongarlic"
    readonly property string emailUrl: "mailto:onion@tal.org"
    readonly property string flattrUrl: "https://flattr.com/submit/auto?user_id=oniongarlic&url=http://www.tal.org/software/onki&title=HelloGPS&language=en_GB&tags=jolla&category=software"


    Dialog {
        id: licenseDialog
        canAccept: false;
        SilicaFlickable {
            anchors.fill: parent
            //contentHeight: cls.height

            Column {
                id: cls
                anchors.fill: parent
                spacing: Theme.paddingMedium
                anchors.margins: Theme.paddingMedium
                PageHeader {
                    title: "License"
                }
                Label {
                    id: nameField
                    text: license
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width;
                    onLinkActivated: Qt.openUrlExternally(link)
                }
            }
        }
    }

    SilicaFlickable {
        id: view
        anchors.fill: parent
        contentHeight: aboutColumn.height

        VerticalScrollDecorator { flickable: view }



        Column {
            id: aboutColumn
            width: parent.width
            spacing: Theme.paddingMedium
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium
            PageHeader {
                title: "Hello GPS ! "+versionStr
                description: "Basic GPS information"
            }

            Label
            {
                id: help
                text: helpText
                styleColor: "#fbe369"
                anchors.topMargin: 20
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeMedium
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                width: parent.width
            }

            Label
            {
                id: copyrightsgpl
                text: "Copyright 2014-2015\nKaj-Michael Lang";
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                textFormat: Text.RichText
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('License')
                onClicked: licenseDialog.open()
            }

            SectionHeader { text: "Contact" }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Hello GPS Project page')
                onClicked: Qt.openUrlExternally(homePageUrl);
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Twitter')
                onClicked: Qt.openUrlExternally(twitterUrl);
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('E-Mail')
                onClicked: Qt.openUrlExternally(emailUrl);
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Flattr it!')
                onClicked: Qt.openUrlExternally(flattrUrl);
            }

            SectionHeader { text: "Help & Contribute" }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('Sources')
                onClicked: Qt.openUrlExternally("https://github.com/oniongarlic/harbour-hellogps")
            }

        }
    }
}
