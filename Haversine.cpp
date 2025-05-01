#include "Haversine.h"
#include <iostream>
const double R = 6371.0;
double haversine(double lat1, double lon1, double lat2, double lon2) {
double latRad1 = lat1 * PI / 180.0;
double lonRad1 = lon1 * PI / 180.0;
double latRad2 = lat2 * PI / 180.0;
double lonRad2 = lon2 * PI / 180.0;

double dLat = latRad2 - latRad1;
double dLon = lonRad2 - lonRad1;

double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
std::cos(latRad1) * std::cos(latRad2) *
std::sin(dLon / 2) * std::sin(dLon / 2);

double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

double distance = R * c;
return distance;}
