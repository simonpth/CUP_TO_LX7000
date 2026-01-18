#include "airporthelper.h"

QString AirportHelper::convertCUPToLXeStyle(int style)
{
    switch(style) {
    case 2:
        return "G"; //grass
    //case 3: outlanding should not be marked as airport
    //    return "G";
    case 4:
        return "G";
    case 5:
        return "C"; //concrete
    default:
        return "I"; //unkown
    }
}

QString AirportHelper::generateLXeFileLine(const Airport &apt)
{
    QString line = "," + sanitizeString(apt.name()) + ",T,";
    line += QString::number(apt.coordinate().latitude()) + ",";
    line += QString::number(apt.coordinate().longitude()) + ",";

    // Elevation - keeping in feet as per previous logic
    QString altString = QString::number(std::round(apt.elevation() * 3.28084));
    if(altString == "nan") altString = "";
    line += altString + ",";

    line += apt.frequency().remove(".") + ",";

    // Runway Length in feet
    line += QString::number(std::round(apt.runwayLength() * 3.28084)) + ",";

    // Runway Direction in degrees
    line += QString::number(apt.runwayDirection()) + ",";

    line += convertCUPToLXeStyle(apt.style()) + ",";
    line += "3,I,254,,," + apt.code();

    return line;
}

QString AirportHelper::sanitizeString(const QString &input)
{
    QString normalized = input.normalized(QString::NormalizationForm_D);
    QString sanitized;
    sanitized.reserve(input.length());

    for (int i = 0; i < normalized.length(); i++) {
        const QChar &c = normalized[i];
        if (c.category() == QChar::Mark_NonSpacing) {
            continue;
        }

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '.' || c == ':') {
            sanitized.append(c);
        } else if (c == ',') {
            sanitized.append('.');
        }
    }
    return sanitized;
}
