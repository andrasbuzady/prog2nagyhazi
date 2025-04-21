#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cmath>


class GpsPoint {
public:
	std::string timestamp;
	double longitude;
	double latitude;
	double elevation;
	GpsPoint(const std::string& ts, double lon, double lat, double ele)
		: timestamp(ts), longitude(lon), latitude(lat), elevation(ele) {
	}
};


class Activity {
public:
    std::string name;
    std::vector<GpsPoint> points;

    virtual ~Activity() {}

    virtual void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string line;

        // Read metadata (implemented in child)
        if (!std::getline(file, line)) {
            std::cerr << "Error reading activity metadata." << std::endl;
            return;
        }
        parseMetadata(line);

        // GPS points
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string ts;
            double lon, lat, ele;
            if (iss >> ts >> lon >> lat >> ele) {
                points.emplace_back(ts, lon, lat, ele);
            }
            else {
                std::cerr << "Error parsing GPS point: " << line << std::endl;
            }
        }
    }

    virtual void parseMetadata(const std::string& line) = 0;

    virtual void printSummary(size_t count = 5) const {
        std::cout << "Activity: " << name << "\n";
        std::cout << "Total points: " << points.size() << "\n";
        for (size_t i = 0; i < std::min(points.size(), count); ++i) {
            const auto& p = points[i];
            std::cout << p.timestamp << " " << p.longitude << " "
                << p.latitude << " " << p.elevation << "\n";
        }
    }
};
class BikeRide : public Activity {
public:
    std::string bikeUsed;

    void parseMetadata(const std::string& line) override {
        name = "Bike Ride";
        bikeUsed = line; // For example, "Cannondale Synapse"
    }

    void printSummary(size_t count = 5) const override {
        std::cout << name << " (Bike: " << bikeUsed << ")\n";
        Activity::printSummary(count);
    }
};

class RunActivity : public Activity {
public:
    std::string shoes;

    void parseMetadata(const std::string& line) override {
        name = "Run";
        shoes = line; // For example, "Nike ZoomX"
    }

    void printSummary(size_t count = 5) const override {
        std::cout << name << " (Shoes: " << shoes << ")\n";
        Activity::printSummary(count);
    }
};


int main() {
    std::vector<Activity*> activities;

    Activity* a1 = new BikeRide();
    a1->loadFromFile(R"(C:\Users\andra\Desktop\prog2nagyhazi\etyekidombokig.txt)");
    activities.push_back(a1);

    Activity* a2 = new RunActivity();
    a2->loadFromFile(R"(C:\Users\andra\Desktop\prog2nagyhazi\Balatoneszakipart.txt)");
    activities.push_back(a2);

    for (const auto* act : activities) {
        act->printSummary();
        std::cout << "\n";
    }

    for (auto* act : activities) {
        delete act;
    }

    return 0;
}
