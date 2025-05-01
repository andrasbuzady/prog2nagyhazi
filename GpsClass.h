#pragma once
class GpsPoint
{
#ifndef GPSPOINT_H
#define GPSPOINT_H

#include <string>

    class GpsPoint {
    public:
        std::string timestamp;
        double longitude;
        double latitude;
        double elevation;

        GpsPoint(const std::string& ts, double lon, double lat, double ele);
    };

#endif // GPSPOINT_H
};

