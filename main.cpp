#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <iomanip>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


constexpr double PI = 3.14159265358979323846;

double haversine(double lat1, double lon1, double lat2, double lon2) {
	const double R = 6371.0;
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
	return distance;
}

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
			std::cerr << "Hiba az adatok beolvasasa soran." << std::endl;
			return;
		}

		parseMetadata(line);

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

	double getHighestElevation(const Activity& activity) const {
		if (activity.points.empty()) return 0.0;
		double maxElevation = std::numeric_limits<double>::lowest(); // itt lehetne double maxElevation = 0.0;

		for (const auto& point : activity.points) {
			if (point.elevation > maxElevation) {
				maxElevation = point.elevation;
			}
		}
		return maxElevation;

	}

	double calculateDistance(const Activity& activity) const {
		double totalDistance = 0.0;
		for (size_t i = 1; i < points.size(); i++) {
			totalDistance += haversine(points[i - 1].latitude, points[i - 1].longitude,
				points[i].latitude, points[i].longitude);
		}
		return totalDistance;
	}


	double calculateElevationSummary(const Activity& activity) const {
		double totalElevation = 0.0;
		for (size_t i = 1; i < points.size(); i++) {
			totalElevation += std::abs(points[i].elevation - points[i - 1].elevation);
		}
		if (totalElevation > 150 && totalElevation < 1500) {
			totalElevation *= 0.43;
			return totalElevation;

		}
		if (totalElevation > 500 && totalElevation < 1000) {
			totalElevation *= 0.35;
			return totalElevation;
		}
		if (totalElevation > 0 && totalElevation < 120) {
			totalElevation *= 0.3;
			return totalElevation;
		}
		return totalElevation;
	}
	virtual void printSummary(size_t count = 5) const {

		std::cout << "GPS pontok szama: " << points.size() << "\n\n";


		std::cout << std::setw(23) << std::left << "Timestamp"
			<< std::setw(12) << "Longitude"
			<< std::setw(12) << "Latitude"
			<< std::setw(12) << "Elevation" << "\n";


		for (size_t i = 0; i < std::min(points.size(), count); ++i) {
			const GpsPoint& p = points[i];
			std::cout << std::setw(20) << std::left << p.timestamp
				<< std::setw(12) << std::right << std::fixed << std::setprecision(6) << p.longitude
				<< std::setw(12) << p.latitude
				<< std::setw(12) << p.elevation << "\n";
		}


		std::cout << std::fixed << std::setprecision(0);
		std::cout << "\nLegmagasabb elert pont: " << getHighestElevation(*this) << " m\n";
		std::cout << "Osszesen megtett szintkulonbseg: " << calculateElevationSummary(*this) << " m\n";
		std::cout << "Osszesen megtett tavolsag: " << calculateDistance(*this) << " km\n";
		std::cout << "=============================\n";
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
		bikeUsed = parts.back();
		name = "";

		for (size_t i = 0; i < parts.size() - 4; ++i) {

			if (!name.empty()) {
				name += " ";
			}
			name += parts[i];
		}

	}


	void printSummary(size_t count = 5) const override {
		std::cout << "=============================\n";
		std::cout << "Kerekpar tura: " << name << "\n";
		std::cout << "Hasznalt kerekpar: " << bikeUsed << "\n\n";

		Activity::printSummary(count);


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
		shoes = parts.back();
		name = "";
		for (size_t i = 0; i < parts.size() - 4; i++) {
			if (!name.empty()) name += " ";
			name += parts[i];
		}

	}

	void printSummary(size_t count = 5) const override {
		std::cout << "=============================\n";
		std::cout << "Futas: " << name << "\n";
		std::cout << "Hasznalt cipo: " << shoes << "\n\n";

		Activity::printSummary(count);


	}
};

void printMenu() {
	std::cout << "1. PrintInfo\n";
	std::cout << "2. Legmagasabb elert szint\n";
	std::cout << "3. Leghosszabb tura\n";
	std::cout << "4. Idoszak alapu kereses \n";
	std::cout << "0. Kilepes\n";
	std::cout << "Adja meg mit szeretne: ";

}






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

	Activity* a4 = new RunActivity();
	a4->fajlbeolvas(R"(C:\Users\andra\Desktop\prog2nagyhazi\futas.txt)");
	activities.push_back(a4);
	/*for (size_t i = 0; i < activities.size(); i++) {
		Activity* act = activities[i];

		act->printSummary();
		std::cout << "\n";
	}*/


	bool running = true;
	while (running) {
	printMenu();
	std::string choice;
	std::getline(std::cin, choice);
	std::cout << "\n\n";

		if (choice == "1") {
			std::cout << "Az informaciok: .\n";
			for (size_t i = 0; i < activities.size(); i++) {
				const Activity* act = activities[i];
				act->printSummary();
				std::cout << "\n";
			}
		}
		else if (choice == "2") {
			std::cout << "A legmagasabb elert szint lett kivalasztva \n";
			const Activity* highestActivity = activities[0];
			double highestElevation = activities[0]->getHighestElevation(*activities[0]);


			for (size_t i = 1; i < activities.size(); i++) {
				double currentElevation = activities[i]->getHighestElevation(*activities[i]);
				if (currentElevation > highestElevation) {
					highestElevation = currentElevation;
					highestActivity = activities[i];
				}
			}
			std::cout << "A tura neve: " << highestActivity->name << "\n";
			std::cout << "A legmagasabb elert szint: " << highestElevation << " m\n";
		}
		else if (choice == "3") {
			std::cout << "A leghosszabb tura lett kivalasztva \n";
			const Activity* longestActivity = activities[0];
			double longestDistance = activities[0]->calculateDistance(*activities[0]);
			for (size_t i = 1; i < activities.size(); i++) {
				double currentDistance = activities[i]->calculateDistance(*activities[i]);
				if (currentDistance > longestDistance) {
					longestDistance = currentDistance;
					longestActivity = activities[i];
				}
			}
			std::cout << "A tura neve: " << longestActivity->name << "\n";
			std::cout << std::fixed << std::setprecision(1);
			std::cout << "A leghosszabb tura: " << longestDistance << " km\n";

		}
		else if (choice == "4")
		{
			std::cout << "Idoszak alapu kereses lett kivalasztva \n";
			std::string year;
			std::cout << "Adja meg az evet: ";
			std::getline(std::cin, year);
			int flag = 0;
			for (size_t i = 0; i < activities.size(); i++) {
				const Activity* act = activities[i];
				if (act->points[0].timestamp.substr(0, 4) == year) {
					act->printSummary();
					std::cout << "\n";
					flag = 1;
				}
			}
			if (flag == 0) {
				std::cout << "Nincs ilyen evben tura!\n";
			}



		}
		else if (choice == "0") {
			std::cout << "Kilepes...\n";
			running = false;
		}

		else {
			std::cout << "Hibas valasztas!\n";

		}
		
		std::cout << "=============================\n";


	}
	for (size_t i = 0; i < activities.size(); i++) {
		Activity* act = activities[i];

		delete act;
	}
	activities.clear(); // meret = 0
	_CrtDumpMemoryLeaks(); // memoriakezeles

	getchar();

	return 0;
}
