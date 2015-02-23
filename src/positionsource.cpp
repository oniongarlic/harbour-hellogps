/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "positionsource.h"
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDebug>

typedef enum {
    GEOCLUE_POSITION_FIELDS_NONE = 0,
    GEOCLUE_POSITION_FIELDS_LATITUDE = 1 << 0,
    GEOCLUE_POSITION_FIELDS_LONGITUDE = 1 << 1,
    GEOCLUE_POSITION_FIELDS_ALTITUDE = 1 << 2
} GeocluePositionFields;

typedef enum {
    GEOCLUE_VELOCITY_FIELDS_NONE = 0,
    GEOCLUE_VELOCITY_FIELDS_SPEED = 1 << 0,
    GEOCLUE_VELOCITY_FIELDS_DIRECTION = 1 << 1,
    GEOCLUE_VELOCITY_FIELDS_CLIMB = 1 << 2
} GeoclueVelocityFields;

typedef enum {
    GEOCLUE_ACCURACY_LEVEL_NONE = 0,
    GEOCLUE_ACCURACY_LEVEL_COUNTRY,
    GEOCLUE_ACCURACY_LEVEL_REGION,
    GEOCLUE_ACCURACY_LEVEL_LOCALITY,
    GEOCLUE_ACCURACY_LEVEL_POSTALCODE,
    GEOCLUE_ACCURACY_LEVEL_STREET,
    GEOCLUE_ACCURACY_LEVEL_DETAILED,
} GeoclueAccuracyLevel;

PositionSource::PositionSource(QObject *parent) :
    QObject(parent),
    valid(false),
    altitude(0),
    lat(0.0),
    lon(0.0),
    satellitesView(0),
    satellitesUsed(0),
    gpsTime(0)
{
    QDBusMessage msg;

    gcmInterface = new QDBusInterface("org.freedesktop.Geoclue.Master", "/org/freedesktop/Geoclue/Master", "", QDBusConnection::sessionBus(), this);
    QDBusReply<QDBusObjectPath> createReply = gcmInterface->call("Create");
    if (createReply.isValid()==false)
        qWarning() << createReply.error();
    else
        qDebug() << createReply.value().path();

    gcInterface = new QDBusInterface( "org.freedesktop.Geoclue.Master", createReply.value().path(), "", QDBusConnection::sessionBus(), this);
    if (gcInterface->isValid()==false)
        qWarning() << "GCMI" << gcInterface->lastError();
}

bool PositionSource::start() {
    QDBusMessage msg;
    bool ok;

    msg=gcInterface->call("PositionStart");
    qDebug() << "PS: " << msg;

    gpsInterface = new QDBusInterface("org.freedesktop.Geoclue.Providers.Hybris", "/org/freedesktop/Geoclue/Providers/Hybris", "", QDBusConnection::sessionBus());
    if (gpsInterface->isValid()==false)
        qWarning() << gcInterface->lastError();

    msg = gpsInterface->call("AddReference");
    qDebug() << msg;

    msg=gpsInterface->call("GetProviderInfo");
    qDebug() << msg;

    msg=gpsInterface->call("GetStatus");
    qDebug() << msg;

    ok=QDBusConnection::sessionBus().connect("org.freedesktop.Geoclue.Position", "/org/freedesktop/Geoclue/Position", "", "PositionChanged", this, SLOT(positionChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCP");

    ok=QDBusConnection::sessionBus().connect("org.freedesktop.Geoclue.Providers.Hybris", "/org/freedesktop/Geoclue/Providers/Hybris", "", "PositionChanged", this, SLOT(gcPositionChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPH");

    ok=QDBusConnection::sessionBus().connect(QString(), QString(), QStringLiteral("org.freedesktop.Geoclue.Satellite"), QStringLiteral("SatelliteChanged"), this, SLOT(gcSatelliteChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPS");

    ok=QDBusConnection::sessionBus().connect(QString(), QString(), QStringLiteral("org.freedesktop.Geoclue.Velocity"), QStringLiteral("VelocityChanged"), this, SLOT(gcVelocityChanged(QDBusMessage)));
    if (ok==false)
        qWarning("Failed to connect GCPV");

    return true;
}

PositionSource::~PositionSource() {
    QDBusMessage msg;

    msg = gpsInterface->call("RemoveReference");
    qDebug() << msg;
}

void PositionSource::gcSatelliteChanged(QDBusMessage msg) {
    QVariantList arguments = msg.arguments();

    uint timestamp = arguments.at(0).toInt();
    satellitesUsed = arguments.at(1).toInt();
    satellitesView = arguments.at(2).toInt();

    qDebug() << "*** Satellites: " << timestamp << "@ " << satellitesUsed << "/" << satellitesView;

    emit satellitesInViewChanged();
    emit satellitesInUseChanged();
}

void PositionSource::gcVelocityChanged(QDBusMessage msg) {
    int vflags;

    QVariantList a = msg.arguments();
    vflags=a.at(0).toInt();

    velocity=(vflags & GEOCLUE_VELOCITY_FIELDS_SPEED) ? a.at(2).toDouble() : 0.0;
    direction=(vflags & GEOCLUE_VELOCITY_FIELDS_DIRECTION) ? a.at(3).toDouble() : 0.0;
    climb=(vflags & GEOCLUE_VELOCITY_FIELDS_CLIMB) ? a.at(4).toDouble() : 0.0;

    qDebug() << "Vel: " << velocity;
    qDebug() << "Dir: " << direction;
    qDebug() << "Cli: " << climb;

    emit velocityChanged(velocity);
    emit directionChanged(direction);
    emit climbChanged(climb);
}

void PositionSource::gcPositionChanged(QDBusMessage msg)
{
    double talt;
    int af;
    double ah,av;
    QVariantList a = msg.arguments();

    flags=a.at(0).toInt();

    bool isValid=(flags & GEOCLUE_POSITION_FIELDS_LATITUDE) && (flags & GEOCLUE_POSITION_FIELDS_LONGITUDE) ? true : false;
    bool isValidAlt=(flags & GEOCLUE_POSITION_FIELDS_ALTITUDE) ? true : false;

    gpsTime=a.at(1).toInt();
    emit tsChanged(gpsTime);

    if (flags & GEOCLUE_POSITION_FIELDS_LATITUDE)
        lat=a.at(2).toDouble();

    if (flags & GEOCLUE_POSITION_FIELDS_LONGITUDE)
        lon=a.at(3).toDouble();

    if (isValidAlt)
        talt=a.at(4).toDouble();
    else
        talt=0.0;

    if (isValid!=valid) {
        valid=isValid;
        emit validChanged();
    }

    if (isValid)
        emit positionChanged(lat, lon);

    if (isValidAlt && talt!=altitude) {
        altitude=talt;
        emit altitudeChanged(altitude);
    }

    qDebug() << "Position: " << valid << " @ " << gpsTime << " : " << "LatLon: " << lat << " / " << lon << " Alt: " << isValidAlt << " : " << altitude;

    const QDBusArgument &aargs = a.at(5).value<QDBusArgument>();
    if (aargs.currentType()==QDBusArgument::StructureType) {
        aargs.beginStructure();
        qDebug("AF");
        aargs >> af;
        qDebug() << af;
        aargs >> ah;
        qDebug() << ah;
        aargs >> av;
        qDebug() << av;
        aargs.endStructure();
    }
    qDebug() << "Accuracy: " << af << " " << ah << " / " << av;
}
