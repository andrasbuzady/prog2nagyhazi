#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <direct.h> 


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

class Ride {
public:
    std::string name;
    std::vector<GpsPoint> points;

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string line;

        // az elso sor a tura elnevezese
        if (std::getline(file, line)) {
            name = line;
        }
        else {
            std::cerr << "Error reading ride name from file: " << filename << std::endl;
            return;
        }

        // osszes gps sor beolvasasa
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string ts;
            double lon, lat, ele;
            if (iss >> ts >> lon >> lat >> ele) {
                points.emplace_back(ts, lon, lat, ele);
            }
            else {
                std::cerr << "Error reading GPS point from line: " << line << std::endl;
            }
        }
    }

    void printSummary(size_t count = 5) const {
        std::cout << "Ride: " << name << "\n";
        std::cout << "Total points: " << points.size() << "\n";
        for (size_t i = 0; i < std::min(points.size(), count); ++i) {
            const auto& p = points[i];
            std::cout << p.timestamp << " " << p.longitude << " "
                << p.latitude << " " << p.elevation << "\n";
        }
    }
};

int main() {
 
    Ride ride;
    ride.loadFromFile(R"(C:\Users\andra\Desktop\prog2nagyhazi\etyekidombokig.txt)");

	if (ride.points.empty()) {
		std::cerr << "No GPS points loaded." << std::endl;
		return 1;
	}
    ride.printSummary();
    Ride ride2;
    ride2.loadFromFile(R"(C:\Users\andra\Desktop\prog2nagyhazi\Balatoneszakipart.txt)");
    ride2.printSummary();

    std::cout << "Ride 2: " << ride2.name << "\n"; //ezt a git change celjabol irom ide

    return 0;
}
