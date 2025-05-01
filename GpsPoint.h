#pragma once
#include <string>
#include <iostream>
class GpsPoint {
public:
	std::string timestamp;
	double longitude;
	double latitude;
	double elevation;
	GpsPoint(const std::string& ts, double lon, double lat, double ele);
};


