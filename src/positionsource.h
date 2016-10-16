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
    Q_PROPERTY(int accuracy READ getAccuracy NOTIFY accuracyChanged)
    Q_PROPERTY(int satellitesInUse READ getSatellitesInUse() NOTIFY satellitesInUseChanged)
    Q_PROPERTY(int satellitesInView READ getSatellitesInView() NOTIFY satellitesInViewChanged)

public:
    explicit PositionSource(QObject *parent = 0);    

    ~PositionSource();
    Q_INVOKABLE bool start();
    Q_INVOKABLE double getAltitude() const { return altitude; }
    Q_INVOKABLE double getVelocity() const { return velocity; }
    Q_INVOKABLE double getDirection() const { return direction; }
    Q_INVOKABLE double getClimb() const { return climb; }
    Q_INVOKABLE double getTs() const { return gpsTime; }
    Q_INVOKABLE double getLatitude() const { return lat; }
    Q_INVOKABLE double getLongitude() const { return lon; }
    Q_INVOKABLE int getSatellitesInView() const { return satellitesView; }
    Q_INVOKABLE int getSatellitesInUse() const { return satellitesUsed; }
    Q_INVOKABLE int getAccuracy() const { return acc; }
    bool getValid() const { return valid; }

signals:
    void validChanged();
    void altitudeChanged(double altitude);
    void velocityChanged(double velocity);
    void climbChanged(double clib);
    void tsChanged(uint ts);
    void accuracyChanged(int accuracy);
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
    int acc;
    double acch;
    double accv;
};

#endif // POSITIONSOURCE_H
