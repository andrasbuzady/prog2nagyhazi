#pragma once
#ifndef RUNACTIVITY_H
#define RUNACTIVITY_H

#include "Activity.h"

class RunActivity : public Activity {
public:
    std::string shoes;

    void parseMetadata(const std::string& line) override;
    void printSummary(size_t count = 5) const override;
};

#endif // RUNACTIVITY_H


