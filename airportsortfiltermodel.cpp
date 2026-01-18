#include "airportsortfiltermodel.h"
#include "airportlistmodel.h"
#include <QGeoCoordinate>
#include <algorithm>

AirportSortFilterModel::AirportSortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    for (int i = 0; i <= 21; ++i) {
        m_allowedStyles.append(i);
    }
}

int AirportSortFilterModel::maxDistance() const
{
    return m_maxDistance;
}

void AirportSortFilterModel::setMaxDistance(int newMaxDistance)
{
    if (m_maxDistance == newMaxDistance)
        return;

    beginFilterChange();
    m_maxDistance = newMaxDistance;
    endFilterChange();

    emit maxDistanceChanged();
}

Airport AirportSortFilterModel::homeAirport() const
{
    return m_homeAirport;
}

void AirportSortFilterModel::setHomeAirport(const Airport &newHomeAirport)
{
    beginFilterChange();
    m_homeAirport = newHomeAirport;
    endFilterChange();

    emit homeAirportChanged();
}

void AirportSortFilterModel::clearHomeAirport()
{
    setHomeAirport(Airport());
}

bool AirportSortFilterModel::useForMap() const
{
    return m_useForMap;
}

void AirportSortFilterModel::setUseForMap(bool newUseForMap)
{
    if (m_useForMap == newUseForMap)
        return;
    m_useForMap = newUseForMap;
    emit useForMapChanged();
}

void AirportSortFilterModel::addAllowedStyle(int style)
{
    if (!m_allowedStyles.contains(style)) {
        beginFilterChange();
        m_allowedStyles.append(style);
        endFilterChange();
        emit allowedStylesChanged();
    }
}

void AirportSortFilterModel::removeAllowedStyle(int style)
{
    if (m_allowedStyles.contains(style)) {
        beginFilterChange();
        m_allowedStyles.removeAll(style);
        endFilterChange();
        emit allowedStylesChanged();
    }
}

bool AirportSortFilterModel::isStyleAllowed(int style) const
{
    return m_allowedStyles.contains(style);
}

int AirportSortFilterModel::distanceForAirportAmount(int amount)
{
    const QGeoCoordinate center = m_homeAirport.coordinate();
    if (amount <= 0 || !center.isValid())
        return -1;

    QAbstractItemModel *model = sourceModel();
    if (!model)
        return -1;

    int sourceRowCount = model->rowCount();
    if (sourceRowCount == 0)
        return -1;

    QVector<double> distances;
    distances.reserve(sourceRowCount);

    for (int i = 0; i < sourceRowCount; ++i) {
        QModelIndex sourceIndex = model->index(i, 0);
        if (sourceIndex.isValid()) {
            int style = model->data(sourceIndex, AirportListModel::StyleRole).toInt();
            if (m_allowedStyles.contains(style)) {
                QGeoCoordinate coord = model->data(sourceIndex, AirportListModel::CoordinateRole).value<QGeoCoordinate>();
                if (coord.isValid()) {
                    distances.append(center.distanceTo(coord));
                }
            }
        }
    }

    if (distances.isEmpty())
        return -1;

    amount = qMin(amount, distances.size());

    std::nth_element(
        distances.begin(),
        distances.begin() + (amount - 1),
        distances.end()
        );

    return qCeil(distances[amount - 1]);
}

int AirportSortFilterModel::getSourceIndexFromIndex(int index)
{
    return this->mapToSource(this->index(index, 0)).row();
}

bool AirportSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    QModelIndex source_index = sourceModel()->index(source_row, 0);
    if (!source_index.isValid()) {
        return false;
    }

    // Get style from AirportListModel using StyleRole
    int style = sourceModel()->data(source_index, AirportListModel::StyleRole).toInt();
    if (!m_allowedStyles.contains(style)) {
        return false;
    }

    // Distance filter
    if (m_maxDistance > -1) {
        if (!m_homeAirport.coordinate().isValid()) {
            return true;
        }

        QGeoCoordinate coord = sourceModel()->data(source_index, AirportListModel::CoordinateRole).value<QGeoCoordinate>();
        if (coord.isValid()) {
            return m_homeAirport.coordinate().distanceTo(coord) <= m_maxDistance;
        }
        return false;
    }

    return true;
}
