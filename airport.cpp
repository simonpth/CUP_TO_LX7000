#include "airport.h"
#include <QMap>
#include <QRegularExpression>

Airport::Airport() {}

// Parse a CUP waypoint line and populate attributes
void Airport::fromCupLine(const QString& line, const QStringList& header)
{
	// Split line into fields, handling quoted commas
	QList<QString> fields;
	int i = 0;
	while (i < line.length()) {
		if (line[i] == '"') {
			int end = line.indexOf('"', i + 1);
			if (end == -1) end = line.length();
			fields.append(line.mid(i + 1, end - i - 1));
			i = end + 1;
			if (i < line.length() && line[i] == ',') ++i;
		} else {
			int end = line.indexOf(',', i);
			if (end == -1) end = line.length();
			fields.append(line.mid(i, end - i).trimmed());
			i = end + 1;
		}
	}

	QMap<QString, QString> map;
	for (int idx = 0; idx < header.size() && idx < fields.size(); ++idx) {
		map[header[idx].toLower()] = fields[idx];
	}

	// Name
	setName(map.value("name"));
	setCode(map.value("code"));
	setCountry(map.value("country"));

	// Latitude/Longitude
	auto parseLatLon = [](const QString& value, bool isLat) -> double {
		// Example: 5147.830N or 01410.467E
		QRegularExpression re(isLat ? R"((\d{2})(\d{2}\.\d+)([NS]))" : R"((\d{3})(\d{2}\.\d+)([EW]))");
		QRegularExpressionMatch m = re.match(value.trimmed());
		if (!m.hasMatch()) return 0.0;
		double deg = m.captured(1).toDouble();
		double min = m.captured(2).toDouble();
		QString dir = m.captured(3);
		double result = deg + min / 60.0;
		if ((isLat && dir == "S") || (!isLat && dir == "W")) result *= -1.0;
		return result;
	};
	double lat = parseLatLon(map.value("lat"), true);
	double lon = parseLatLon(map.value("lon"), false);
	setCoordinate(QGeoCoordinate(lat, lon));

	// Elevation
	setElevationString(map.value("elev"));

	// Style
	setStyle(map.value("style").toInt());

	// Runway Direction
	setRunwayDirection(map.value("rwdir").toInt());

	// Runway Length
	setRunwayLengthString(map.value("rwlen"));

	// Runway Width
	setRunwayWidthString(map.value("rwwidth"));

	// Frequency
	setFrequency(map.value("freq"));

	// Description
	setDescription(map.value("desc"));

	// Userdata
	setUserdata(map.value("userdata"));
}

QString Airport::name() const { return m_name; }
void Airport::setName(const QString& name) { m_name = name; }

QString Airport::code() const { return m_code; }
void Airport::setCode(const QString& code) { m_code = code; }

QString Airport::country() const { return m_country; }
void Airport::setCountry(const QString& country) { m_country = country; }

QGeoCoordinate Airport::coordinate() const { return m_coordinate; }
void Airport::setCoordinate(const QGeoCoordinate& coordinate) { m_coordinate = coordinate; }

double Airport::elevation() const { return m_elevation; }
void Airport::setElevation(double elevation, const QString& unit) {
	m_elevation = toMeters(elevation, unit);
}
void Airport::setElevationString(const QString& elevStr) {
	QString unit;
	double value = parseValueWithUnit(elevStr, &unit);
	m_elevation = toMeters(value, unit);
}

int Airport::style() const { return m_style; }
void Airport::setStyle(int style) { m_style = style; }

int Airport::runwayDirection() const { return m_runwayDirection; }
void Airport::setRunwayDirection(int direction) { m_runwayDirection = direction; }

double Airport::runwayLength() const { return m_runwayLength; }
void Airport::setRunwayLength(double length, const QString& unit) {
	m_runwayLength = toMeters(length, unit);
}
void Airport::setRunwayLengthString(const QString& lenStr) {
	QString unit;
	double value = parseValueWithUnit(lenStr, &unit);
	m_runwayLength = toMeters(value, unit);
}

double Airport::runwayWidth() const { return m_runwayWidth; }
void Airport::setRunwayWidth(double width, const QString& unit) {
	m_runwayWidth = toMeters(width, unit);
}
void Airport::setRunwayWidthString(const QString& widthStr) {
	QString unit;
	double value = parseValueWithUnit(widthStr, &unit);
	m_runwayWidth = toMeters(value, unit);
}

QString Airport::frequency() const { return m_frequency; }
void Airport::setFrequency(const QString& frequency) { m_frequency = frequency; }

QString Airport::description() const { return m_description; }
void Airport::setDescription(const QString& description) { m_description = description; }

QString Airport::userdata() const { return m_userdata; }
void Airport::setUserdata(const QString& userdata) { m_userdata = userdata; }

// Helper: convert value to meters
double Airport::toMeters(double value, const QString& unit) {
	QString u = unit.trimmed().toLower();
	if (u == "m" || u.isEmpty()) {
		return value;
	} else if (u == "ft") {
		return value * 0.3048;
	} else if (u == "nm") {
		return value * 1852.0;
	} else if (u == "ml") {
		return value * 1609.344;
	}
	return value; // fallback
}

// Helper: parse value and unit from string
double Airport::parseValueWithUnit(const QString& str, QString* outUnit) {
	QRegularExpression re(R"(([-+]?[0-9]*\.?[0-9]+)\s*([a-zA-Z]*)?)");
	QRegularExpressionMatch match = re.match(str.trimmed());
	double value = 0.0;
	QString unit;
	if (match.hasMatch()) {
		value = match.captured(1).toDouble();
		unit = match.captured(2);
	}
	if (outUnit) *outUnit = unit;
	return value;
}

bool Airport::operator==(const Airport& other) const {
    return other.coordinate().isValid() && m_coordinate.isValid() && m_coordinate.distanceTo(other.coordinate()) <= 7.0; // 6 or 7 ?
}

bool Airport::equals(const Airport &other)
{
    return other == *this;
}
