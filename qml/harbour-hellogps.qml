import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.hellogps 1.0
import "pages"
import "cover"

ApplicationWindow
{
    id: root

    property PositionSource gps: rootGps    
    readonly property int inView: rootGps.satellitesInView
    readonly property int inUse: rootGps.satellitesInUse
    readonly property bool valid: rootGps.valid

    initialPage: Component {
        MainPage {

        }
    }

    cover: Component {
        CoverPage {

        }
    }

    PositionSource {
        id: rootGps
    }

    Component.onCompleted: {
        rootGps.start();
    }
}


