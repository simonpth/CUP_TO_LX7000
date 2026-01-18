#ifndef AIRPORTSORTFILTERMODEL_H
#define AIRPORTSORTFILTERMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QList>
#include <QGeoCoordinate>

#include "airport.h"

class AirportSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged FINAL)
    Q_PROPERTY(Airport homeAirport READ homeAirport WRITE setHomeAirport NOTIFY homeAirportChanged FINAL)
    Q_PROPERTY(bool useForMap READ useForMap WRITE setUseForMap NOTIFY useForMapChanged FINAL)

public:
    AirportSortFilterModel(QObject *parent = nullptr);

    int maxDistance() const;
    void setMaxDistance(int newMaxDistance);

    Airport homeAirport() const;
    void setHomeAirport(const Airport &newHomeAirport);
    Q_INVOKABLE void clearHomeAirport();

    bool useForMap() const;
    void setUseForMap(bool newUseForMap);

    Q_INVOKABLE void addAllowedStyle(int style);
    Q_INVOKABLE void removeAllowedStyle(int style);
    Q_INVOKABLE bool isStyleAllowed(int style) const;

    Q_INVOKABLE int distanceForAirportAmount(int amount);

    Q_INVOKABLE int getSourceIndexFromIndex(int index);

signals:
    void maxDistanceChanged();
    void homeAirportChanged();
    void useForMapChanged();
    void allowedStylesChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    int m_maxDistance = 0;
    Airport m_homeAirport;
    QList<int> m_allowedStyles;

    bool m_useForMap = true;
};

#endif // AIRPORTSORTFILTERMODEL_H
