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

	virtual ~Activity() {}  //virtuális, hiszen a leszármazottak esetén akarom meghivni a destruktort

	virtual void fajlbeolvas(const std::string& filename) {
		std::ifstream file(filename);
		if (!file) {
			std::cerr << "Hiba a fajl eseteben: " << filename << std::endl;
			return;
		}

		std::string line;


		if (!std::getline(file, line)) {
			std::cerr << "Error reading activity metadata." << std::endl;
			return;
		}
		//metadata = data about data 
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
				std::cerr << "Hiba a GPS pont feldolgozasok soran: " << line << std::endl;
			}
		}
	}
	virtual void parseMetadata(const std::string& line) = 0;



	virtual void printSummary(size_t count = 5) const {
		std::cout << "Activity: " << name << "\n";
		std::cout << "Total points: " << points.size() << "\n";
		for (size_t i = 0; i < std::min(points.size(), count); ++i) {
			const GpsPoint& p = points[i];
			std::cout << p.timestamp << " " << p.longitude << " "
				<< p.latitude << " " << p.elevation << "\n";
		}
	}


	double getHighestElevation(const Activity& activity) const {
		double maxElevation = std::numeric_limits<double>::lowest(); // itt lehetne double maxElevation = 0.0;

		for (const auto& point : activity.points) {
			if (point.elevation > maxElevation) {
				maxElevation = point.elevation;
			}
		}
		return maxElevation;

	}

	double calculateElevationSummary(const Activity& activity) const {
		double totalElevation = 0.0;
		for (size_t i = 1; i < points.size(); i++) {
			totalElevation += std::abs(points[i].elevation - points[i - 1].elevation);
		}
		return totalElevation;
	}

};
class BikeRide : public Activity {
public:
	std::string bikeUsed;

	void parseMetadata(const std::string& line) override {
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
		std::string day = parts[parts.size() - 4];
		std::string month = parts[parts.size() - 3];
		std::string year = parts[parts.size() - 2];
		bikeUsed = parts[parts.size() - 1];
		for (size_t i = 0; i < parts.size() - 4; i++) {
			name += parts[i];
			if (i < parts.size() - 5)
				name += " ";
		}
	}



	void printSummary(size_t count = 5) const override {
		std::cout << name << "\t" << "Hasznalt kerekpar : " << bikeUsed << "\n";
		Activity::printSummary(count);
		std::cout << "Legmagasabb tengerszint feletti magassag: " << getHighestElevation(*this) << "\n";
		std::cout << "Osszes megtett szint:" << calculateElevationSummary(*this) << "\n";
	}
};



class RunActivity : public Activity {
public:
	std::string shoes;

	void parseMetadata(const std::string& line) override {
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
		name = "Run: ";
		for (unsigned int i = 0; i < parts.size() - 4; i++) {
			name += parts[i];
			if (i < parts.size() - 5) { name += " "; }
		}
		shoes = parts.back();
	}

	void printSummary(size_t count = 5) const override {
		std::cout << name << "\t" << "Hasznalt cipo: " << shoes << "\n";
		Activity::printSummary(count);
	}
};





int main() {
	std::vector<Activity*> activities;

	Activity* a1 = new BikeRide();
	a1->fajlbeolvas(R"(C:\Users\andra\Desktop\prog2nagyhazi\etyekidombokig.txt)");
	activities.push_back(a1);

	Activity* a2 = new BikeRide();
	a2->fajlbeolvas(R"(C:\Users\andra\Desktop\prog2nagyhazi\Balatoneszakipart.txt)");
	activities.push_back(a2);

	Activity* a3 = new RunActivity();
	a3->fajlbeolvas(R"(C:\Users\andra\Desktop\prog2nagyhazi\margitszigetkor.txt)");
	activities.push_back(a3);
	for (const auto* act : activities) {
		act->printSummary();
		std::cout << "\n";
	}

	for (auto* act : activities) {
		delete act;
	}
	getchar();

	printf("size max: %zu\n", SIZE_MAX);
	return 0;
}
