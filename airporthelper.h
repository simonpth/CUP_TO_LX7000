#ifndef AIRPORTHELPER_H
#define AIRPORTHELPER_H

#include <QString>
#include "airport.h"

class AirportHelper
{
public:
    static QString convertCUPToLXeStyle(int style);
    static QString generateLXeFileLine(const Airport& apt);
    static QString sanitizeString(const QString& input);
};

#endif // AIRPORTHELPER_H
