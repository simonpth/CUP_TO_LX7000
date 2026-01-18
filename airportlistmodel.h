#ifndef AIRPORTLISTMODEL_H
#define AIRPORTLISTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "airport.h"
#include <QList>
#include <QString>

class AirportListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum AirportRoles {
        NameRole = Qt::UserRole + 1,
        CodeRole,
        CountryRole,
        CoordinateRole,
        ElevationRole,
        StyleRole,
        RunwayDirectionRole,
        RunwayLengthRole,
        RunwayWidthRole,
        FrequencyRole,
        DescriptionRole,
        UserdataRole
    };

    explicit AirportListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Functionality from AirportList:
    Q_INVOKABLE bool readCupFile(const QString& filePath);
    Q_INVOKABLE void clearAirports();
    const QList<Airport>& airports() const { return m_airports; }

    Q_INVOKABLE QGeoCoordinate averagePosition() const;

    Q_INVOKABLE int indexFromName(const QString &name) const;
    Q_INVOKABLE Airport at(int index) const;

    Q_INVOKABLE void addPossibleStyle(int style);
    Q_INVOKABLE void removePossibleStyle(int style);
    Q_INVOKABLE bool isStylePossible(int style) const;

    Q_INVOKABLE void updateAirport(int index, const QString &name, const QString &code, const QString &country,
                                       double latitude, double longitude, double elevation, int style,
                                       int runwayDirection, double runwayLength, double runwayWidth,
                                       const QString &frequency, const QString &description, const QString &userdata);

    signals:

        void countChanged();

        void possibleStylesChanged();

    private:

        QList<Airport> m_airports;
        QList<int> m_possibleStyles;

    };

#endif // AIRPORTLISTMODEL_H
