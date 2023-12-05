#include "util.hpp"

#include <iostream>
#include <conio.h>
#include <string>

Matrix matrix;
Algorithms algo;

void algorithmMenu() {
	char option;
	do {
		std::cout << "\n==== URUCHOM ALGORYTM ===\n";
		std::cout << "1.Brute force (ST)\n";
		std::cout << "2.Brute force (MT)\n";
		std::cout << "3.Podzial i ograniczenia\n";
		std::cout << "4.Programowanie dynamiczne\n";
		std::cout << "5.Benchmark\n";
		std::cout << "0.Powrot\n";
		std::cout << "Podaj opcje:";
		option = _getche();
		std::cout << std::endl;

		switch (option) {
		case '1':
			algo.displayResults();
			break;

		case '2':
			algo.displayResults();
			break;

		case '3':
			algo.displayResults();
			break;

		case '4':
			algo.displayResults();
			break;

		case '5':
			break;
		}
	} while (option != '0');
}

int main() {
	char option;
	std::string fileName;
	int value;

	do {
		std::cout << "\n==== MENU GLOWNE ===\n";
		std::cout << "1.Wczytaj z pliku\n";
		std::cout << "2.Wprowadz kryterium stopu\n";
		std::cout << "3.Wybor sasiedztwa\n";
		std::cout << "4.Uruchom algorytm TS\n";
		std::cout << "5.Ustaw wspolczynnik zmiany temperatury\n";
		std::cout << "6.Uruchom algorytm SW\n";
		std::cout << "0.Wyjdz\n";
		std::cout << "Podaj opcje:";
		option = _getche();
		std::cout << std::endl;

		switch (option) {
		case '1':
			std::cout << " Podaj nazwe zbioru:";
			std::cin >> fileName;
			matrix.loadFromFile(fileName);
			// matrix.display();
			clear();
			break;

		case '2':
			std::cout << "Podaj ilosc wierzcholkow:";
			std::cin >> value;
			clear();
			break;

		case '3':
			clear();
			matrix.display();
			break;

		case '4':
			algorithmMenu();
			break;
		}
	} while (option != '0');

	return 0;
}