# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-hellogps

CONFIG += sailfishapp

QT += dbus

SOURCES += src/harbour-hellogps.cpp src/positionsource.cpp \
    src/track.cpp

HEADERS += src/positionsource.h \
    src/track.h

OTHER_FILES += qml/harbour-hellogps.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-hellogps.changes.in \
    rpm/harbour-hellogps.spec \
    rpm/harbour-hellogps.yaml \
    translations/*.ts \
    harbour-hellogps.desktop \
    qml/pages/MainPage.qml \
    qml/pages/AboutPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-hellogps-de.ts

