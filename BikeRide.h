#pragma once
#ifndef BIKERIDE_H
#define BIKERIDE_H

#include "Activity.h"

class BikeRide : public Activity {
public:
    std::string bikeUsed;

    void parseMetadata(const std::string& line) override;
    void printSummary(size_t count = 5) const override;
};

#endif // BIKERIDE_H
