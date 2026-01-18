#ifndef AIRPORT_H
#define AIRPORT_H

#include <QObject>
#include <QString>
#include <QGeoCoordinate>
#include <QMetaType>
#include <qqmlintegration.h>

class Airport
{
    Q_GADGET

    QML_NAMED_ELEMENT(airport)

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString code READ code WRITE setCode)
    Q_PROPERTY(QString country READ country WRITE setCountry)
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate)
    Q_PROPERTY(double elevation READ elevation WRITE setElevation)
    Q_PROPERTY(int style READ style WRITE setStyle)
    Q_PROPERTY(int runwayDirection READ runwayDirection WRITE setRunwayDirection)
    Q_PROPERTY(double runwayLength READ runwayLength WRITE setRunwayLength)
    Q_PROPERTY(double runwayWidth READ runwayWidth WRITE setRunwayWidth)
    Q_PROPERTY(QString frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString userdata READ userdata WRITE setUserdata)

public:
    Airport();

    void fromCupLine(const QString& line, const QStringList& header);

    QString name() const;
    void setName(const QString& name);

    QString code() const;
    void setCode(const QString& code);

    QString country() const;
    void setCountry(const QString& country);

    QGeoCoordinate coordinate() const;
    void setCoordinate(const QGeoCoordinate& coordinate);

    double elevation() const; // always meters
    void setElevation(double elevation, const QString& unit = "m");
    void setElevationString(const QString& elevStr);

    int style() const;
    void setStyle(int style);

    int runwayDirection() const;
    void setRunwayDirection(int direction);

    double runwayLength() const; // always meters
    void setRunwayLength(double length, const QString& unit = "m");
    void setRunwayLengthString(const QString& lenStr);

    double runwayWidth() const; // always meters
    void setRunwayWidth(double width, const QString& unit = "m");
    void setRunwayWidthString(const QString& widthStr);

    QString frequency() const;
    void setFrequency(const QString& frequency);

    QString description() const;
    void setDescription(const QString& description);

    QString userdata() const;
    void setUserdata(const QString& userdata);

    bool operator==(const Airport& other) const;
    Q_INVOKABLE bool equals(const Airport& other);

private:
    QString m_name;
    QString m_code;
    QString m_country;
    QGeoCoordinate m_coordinate;
    double m_elevation = 0.0; // meters
    int m_style = 0; // CUP style
    int m_runwayDirection = 0;
    double m_runwayLength = 0.0; // meters
    double m_runwayWidth = 0.0; // meters
    QString m_frequency;
    QString m_description;
    QString m_userdata;

    // Helper for unit conversion
    static double toMeters(double value, const QString& unit);
        static double parseValueWithUnit(const QString& str, QString* outUnit = nullptr);
};

    Q_DECLARE_METATYPE(Airport)

#endif // AIRPORT_H
