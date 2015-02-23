import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cb

    CoverPlaceholder {
        id: coverph
        text: "Hello GPS\n"+root.gps.valid===true ? "Fix" : "No fix"
        icon.source: "icon.png"
    }
}


