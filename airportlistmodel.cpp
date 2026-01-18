#include "airportlistmodel.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

AirportListModel::AirportListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int AirportListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_airports.size());
}

QVariant AirportListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();

    const Airport& airport = m_airports.at(index.row());

    switch (role) {
    case NameRole:
    case Qt::DisplayRole:
        return airport.name();
    case CodeRole:
        return airport.code();
    case CountryRole:
        return airport.country();
    case CoordinateRole:
        return QVariant::fromValue(airport.coordinate());
    case ElevationRole:
        return airport.elevation();
    case StyleRole:
        return airport.style();
    case RunwayDirectionRole:
        return airport.runwayDirection();
    case RunwayLengthRole:
        return airport.runwayLength();
    case RunwayWidthRole:
        return airport.runwayWidth();
    case FrequencyRole:
        return airport.frequency();
    case DescriptionRole:
        return airport.description();
    case UserdataRole:
        return airport.userdata();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AirportListModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {CodeRole, "code"},
        {CountryRole, "country"},
        {CoordinateRole, "coordinate"},
        {ElevationRole, "elevation"},
        {StyleRole, "style"},
        {RunwayDirectionRole, "runwayDirection"},
        {RunwayLengthRole, "runwayLength"},
        {RunwayWidthRole, "runwayWidth"},
        {FrequencyRole, "frequency"},
        {DescriptionRole, "description"},
        {UserdataRole, "userdata"}
    };
}

bool AirportListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_airports.insert(row + i, Airport());
    }
    endInsertRows();
    emit countChanged();
    return true;
}

bool AirportListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        m_airports.removeAt(row);
    }
    endRemoveRows();
    emit countChanged();
    return true;
}

void AirportListModel::clearAirports()
{
    if (!m_airports.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_airports.size() - 1);
        m_airports.clear();
        endRemoveRows();
        emit countChanged();
    }
}

bool AirportListModel::readCupFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString headerLine;
    while (!in.atEnd()) {
        headerLine = in.readLine();
        if (headerLine.trimmed().isEmpty())
            continue;
        if (headerLine.startsWith("-----Related Tasks-----")) {
            file.close();
            return false;
        }
        break;
    }
    if (headerLine.isNull() || headerLine.trimmed().isEmpty()) {
        file.close();
        return false;
    }

    QStringList header = headerLine.split(',', Qt::KeepEmptyParts);
    for (QString& h : header)
        h = h.trimmed().toLower();

    // Parse new airports
    QList<Airport> newAirports;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;
        if (line.startsWith("-----Related Tasks-----"))
            break;

        Airport airport;
        airport.fromCupLine(line, header);
        if (!m_airports.contains(airport)) {
            newAirports.append(airport);
            addPossibleStyle(airport.style());
        }
    }

    // Insert new airports efficiently
    if (!newAirports.isEmpty()) {
        int insertPosition = m_airports.size();
        beginInsertRows(QModelIndex(), insertPosition, insertPosition + newAirports.size() - 1);
        m_airports.append(newAirports);
        endInsertRows();
        emit countChanged();
    }

    file.close();
    return true;
}

QGeoCoordinate AirportListModel::averagePosition() const
{
    if (m_airports.isEmpty())
        return QGeoCoordinate();

    double totalLatitude = 0.0;
    double totalLongitude = 0.0;

    for (const Airport& airport : m_airports) {
        totalLatitude += airport.coordinate().latitude();
        totalLongitude += airport.coordinate().longitude();
    }

    double avgLatitude = totalLatitude / m_airports.size();
    double avgLongitude = totalLongitude / m_airports.size();

    return QGeoCoordinate(avgLatitude, avgLongitude);
}

int AirportListModel::indexFromName(const QString &name) const
{
    for (int i = 0; i < m_airports.length(); ++i) {
        if (m_airports.at(i).name() == name) {
            return i;
        }
    }
    return -1;
}

Airport AirportListModel::at(int index) const
{
    if(index >= 0 && index < m_airports.length()) {
        return m_airports[index];
    }
    return Airport();
}

void AirportListModel::addPossibleStyle(int style)
{
    if(!m_possibleStyles.contains(style)) {
        m_possibleStyles.append(style);
        emit possibleStylesChanged();
    }
}

void AirportListModel::removePossibleStyle(int style)
{
    m_possibleStyles.removeAll(style);
    m_possibleStyles.squeeze();
    emit possibleStylesChanged();
}

bool AirportListModel::isStylePossible(int style) const
{
    return m_possibleStyles.contains(style);
}

void AirportListModel::updateAirport(int index, const QString &name, const QString &code, const QString &country, double latitude, double longitude, double elevation, int style, int runwayDirection, double runwayLength, double runwayWidth, const QString &frequency, const QString &description, const QString &userdata)
{
    if (index < 0 || index >= m_airports.count())
        return;

    Airport &airport = m_airports[index];
    airport.setName(name);
    airport.setCode(code);
    airport.setCountry(country);
    airport.setCoordinate(QGeoCoordinate(latitude, longitude));
    airport.setElevation(elevation);
    airport.setStyle(style);
    airport.setRunwayDirection(runwayDirection);
    airport.setRunwayLength(runwayLength);
    airport.setRunwayWidth(runwayWidth);
    airport.setFrequency(frequency);
    airport.setDescription(description);
    airport.setUserdata(userdata);

    QModelIndex modelIndex = this->index(index, 0);
    emit dataChanged(modelIndex, modelIndex, roleNames().keys().toVector());
}



