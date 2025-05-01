#include "BikeRide.h"
#include "BikeRide.h"
#include <iostream>
#include <sstream>
#include <vector>

void BikeRide::parseMetadata(const std::string& line) {
    std::istringstream iss(line);
    std::string word;
    std::vector<std::string> parts;

    while (iss >> word) {
        parts.push_back(word);
    }
    if (parts.size() < 5) {
        std::cerr << "hibas adat: " << line << std::endl;
        return;
    }
    bikeUsed = parts.back();
    name = "";

    for (size_t i = 0; i < parts.size() - 4; ++i) {
        if (!name.empty()) {
            name += " ";
        }
        name += parts[i];
    }
}

void BikeRide::printSummary(size_t count) const {
    std::cout << "=============================\n";
    std::cout << "Kerekpar tura: " << name << "\n";
    std::cout << "Hasznalt kerekpar: " << bikeUsed << "\n\n";

    Activity::printSummary(count);
}
