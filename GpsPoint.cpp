#include "GpsPoint.h"
#include <iostream>

GpsPoint::GpsPoint(const std::string& ts, double lon, double lat, double ele)
    : timestamp(ts), longitude(lon), latitude(lat), elevation(ele) {
}
;

