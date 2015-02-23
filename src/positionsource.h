#ifndef POSITIONSOURCE_H
#define POSITIONSOURCE_H

#include <QObject>
#include <QDBusInterface>
#include <QMap>

class PositionSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ getValid NOTIFY validChanged)
    Q_PROPERTY(double altitude READ getAltitude NOTIFY altitudeChanged)
    Q_PROPERTY(double velocity READ getVelocity() NOTIFY velocityChanged)
    Q_PROPERTY(double direction READ getDirection() NOTIFY directionChanged)
    Q_PROPERTY(double climb READ getClimb() NOTIFY climbChanged)
    Q_PROPERTY(int ts READ getTs() NOTIFY tsChanged)
    Q_PROPERTY(int satellitesInUse READ getSatellitesInUse() NOTIFY satellitesInUseChanged)
    Q_PROPERTY(int satellitesInView READ getSatellitesInView() NOTIFY satellitesInViewChanged)

public:
    explicit PositionSource(QObject *parent = 0);    

    ~PositionSource();
    Q_INVOKABLE bool start();
    Q_INVOKABLE double getAltitude() { return altitude; }
    Q_INVOKABLE double getVelocity() { return velocity; }
    Q_INVOKABLE double getDirection() { return direction; }
    Q_INVOKABLE double getClimb() { return climb; }
    Q_INVOKABLE double getTs() { return gpsTime; }
    Q_INVOKABLE double getLatitude() { return lat; }
    Q_INVOKABLE double getLongitude() { return lon; }
    Q_INVOKABLE int getSatellitesInView() { return satellitesView; }
    Q_INVOKABLE int getSatellitesInUse() { return satellitesUsed; }
    bool getValid() { return valid; }

signals:
    void validChanged();
    void altitudeChanged(double altitude);
    void velocityChanged(double velocity);
    void climbChanged(double clib);
    void tsChanged(int ts);
    void directionChanged(double direction);
    void positionChanged(double lat, double lon);
    void satellitesInViewChanged();
    void satellitesInUseChanged();    

public slots:       

protected slots:
    void gcSatelliteChanged(QDBusMessage msg);
    void gcPositionChanged(QDBusMessage msg);
    void gcVelocityChanged(QDBusMessage msg);

private:
    QDBusInterface *gpsInterface;
    QDBusInterface *gcmInterface;
    QDBusInterface *gcInterface;

    bool valid;    
    double altitude;
    double lat,lon;
    double velocity;
    double direction;
    double climb;
    int flags;    
    int satellitesView;
    int satellitesUsed;
    int gpsTime;
    int acch;
    int accv;
};

#endif // POSITIONSOURCE_H
