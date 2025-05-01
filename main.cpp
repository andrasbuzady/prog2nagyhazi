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
#include "Activity.h"
#include "BikeRide.h"
#include "RunActivity.h"
#include "GpsPoint.h"





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
			std::cout << "Az informaciok: \n";
			for (size_t i = 0; i < activities.size(); i++) {
				const Activity* act = activities[i];
				act->printSummary();
				std::cout << "\n\n\n";
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
			std::cout << "\n\n\n";
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
			std::cout << "\n\n\n";

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
			std::cout << "\n\n\n";
		}

		else {
			std::cout << "Hibas valasztas!\n";
			std::cout << "\n\n\n";

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
