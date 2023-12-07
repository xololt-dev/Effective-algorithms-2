#include "util.hpp"

#include <iostream>
#include <conio.h>
#include <string>

Matrix matrix;
Algorithms algo;

void neighboursMenu() {
	char option;
	do {
		std::cout << "\n==== SASIEDZTWO ===\n";
		std::cout << "1.Odwrotna kolejnosc\n";
		std::cout << "2.Zamiana miejsc\n";
		std::cout << "3.Wstaw w miejsce\n";
		std::cout << "4.Wstaw podsciezke\n";
		std::cout << "0.Powrot\n";
		std::cout << "Podaj opcje:";
		option = _getche();
		std::cout << std::endl;

		switch (option) {
		case '1':
			algo.setNeighbourhoodType(INVERSE);
			return;

		case '2':
			algo.setNeighbourhoodType(SWAP);
			return;

		case '3':
			algo.setNeighbourhoodType(INSERT);
			return;

		case '4':
			algo.setNeighbourhoodType(INSERT_SUB);
			return;
		}
	} while (option != '0');
}

void stopCriteriumMenu() {
	char option;
	do {
		std::cout << "\n==== KRYTERIUM STOPU ===\n";
		std::cout << "1.Okreslona liczba iteracji\n";
		std::cout << "2.Brak poprawy po n iteracjach\n";
		std::cout << "3.Okreslona liczba zaakceptowanych rozwiazan ponizej n progu\n";
		std::cout << "4.Akceptowalny poziom bledu upper/lower bound\n";
		std::cout << "5.Staly czas wykonywania\n";
		std::cout << "0.Powrot\n";
		std::cout << "Podaj opcje:";
		option = _getche();
		std::cout << std::endl;

		switch (option) {
		case '1':
			algo.setNeighbourhoodType(INVERSE);
			return;

		case '2':
			algo.setNeighbourhoodType(SWAP);
			return;

		case '3':
			algo.setNeighbourhoodType(INSERT);
			return;

		case '4':
			algo.setNeighbourhoodType(INSERT_SUB);
			return;
		}
	} while (option != '0');
}

int main() {
	char option;
	std::string fileName;
	int value;

	algo.initRandom();

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
			break;

		case '2':
			std::cout << "Podaj dlugosc wykonania (s):";
			std::cin >> value;
			algo.setStopCriterium(value);
			clear();
			break;

		case '3':
			clear();
			neighboursMenu();
			break;

		case '4':
			
			break;
		
		case '5':
			std::cout << "Podaj wspolczynnik schladzania:";
			float a;
			std::cin >> a;
			algo.setCoolingConstant(a);
			clear();
			break;
		
		case '6':
			algo.setStopCriterium(10);
			for (double i = 1.0; i < 20; i *= 2.0) {
				algo.simulatedAnnealing((Matrix*)&matrix, i, 168, 170*169);
				algo.displayResults();
			}

			// algo.simulatedAnnealing((Matrix*) &matrix);
			// algo.displayResults();
			break;
		}
	} while (option != '0');

	return 0;
}