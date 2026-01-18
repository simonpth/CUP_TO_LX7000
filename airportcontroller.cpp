#include "airportcontroller.h"

#include "airporthelper.h"
#include <QFile>

AirportController::AirportController(QObject *parent)
    : QObject{parent}
{
    m_airportListModel = new AirportListModel(this);
    m_airportSortFilterModel = new AirportSortFilterModel(this);
    m_airportSortFilterModel->setSourceModel(m_airportListModel);
}

AirportListModel *AirportController::airportListModel() const
{
    return m_airportListModel;
}

void AirportController::setHomeAirportByIndex(int index)
{
    if (index < 0 || index >= m_airportListModel->rowCount()) {
        return;
    }
    Airport homeApt = m_airportListModel->at(index);
    m_airportSortFilterModel->setHomeAirport(homeApt);
}

void AirportController::exportAirports(const QUrl &fileUrl)
{
    if(!fileUrl.isLocalFile()) {
        return;
    }
    QString filePath = fileUrl.toLocalFile();
    if(filePath.endsWith("txt")) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&file);
            out << "*,UserAptAPT,UserAptAPTTYPE,UserAptLAT,UserAptLON,UserAptALT,UserAptF,UserAptRW(3digit),UserAptRWDIR(2digit),"
                   "UserAptRWTYPE,UserAptCALT,UserAptCDIR,UserAptCODE,UserAptRWN,UserAptAPT_SHORT,UserAptLABEL\n";

            for (int row = 0; row < m_airportSortFilterModel->rowCount(); ++row) {
                Airport airport = m_airportListModel->airports().at(m_airportSortFilterModel->getSourceIndexFromIndex(row));
                out << AirportHelper::generateLXeFileLine(airport) << "\n";
            }
        }

        file.close();
    }
}

AirportSortFilterModel *AirportController::airportSortFilterModel() const
{
    return m_airportSortFilterModel;
}
