#ifndef AIRPORTCONTROLLER_H
#define AIRPORTCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QUrl>

#include "airportlistmodel.h"
#include "airportsortfiltermodel.h"

class AirportController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(AirportListModel *airportListModel READ airportListModel CONSTANT FINAL)
    Q_PROPERTY(AirportSortFilterModel *airportSortFilterModel READ airportSortFilterModel CONSTANT FINAL)

public:
    explicit AirportController(QObject *parent = nullptr);

    AirportListModel *airportListModel() const;

    Q_INVOKABLE void setHomeAirportByIndex(int index);
    Q_INVOKABLE void exportAirports(const QUrl &fileUrl);

    AirportSortFilterModel *airportSortFilterModel() const;

signals:

private:
    AirportListModel *m_airportListModel;
    AirportSortFilterModel *m_airportSortFilterModel;
};

#endif // AIRPORTCONTROLLER_H
