#pragma once
#ifndef ACTIVITY_H
#define ACTIVITY_H
#include <iostream>
#include <string>
#include <vector>
#include "GpsPoint.h"

class Activity {
public:
    std::string name;
    std::vector<GpsPoint> points;

    virtual ~Activity();
    virtual void fajlbeolvas(const std::string& filename);
    virtual void parseMetadata(const std::string& line) = 0;
    virtual void printSummary(size_t count = 5) const;

    double getHighestElevation(const Activity& activity) const;
    double calculateDistance(const Activity& activity) const;
    double calculateElevationSummary(const Activity& activity) const;
};

#endif 


