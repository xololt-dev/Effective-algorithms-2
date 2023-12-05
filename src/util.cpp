#include "util.hpp"
#include <iostream>
#include <string>

void Matrix::loadFromFile(std::string fileName) {
	int n = fileName.find(".txt");

	if (n != std::string::npos
		&& fileName.length() - n == 4) {
		loadFromTxt(fileName);
		return;
	}

	n = fileName.find(".atsp");

	if (n != std::string::npos 
		&& fileName.length() - n == 5) {
		loadFromATSP(fileName);
	}

	std::cout << "Nie mozna otworzyc pliku!\n";
}

void Matrix::loadFromTxt(std::string fileName) {
	std::fstream file;
	file.open(fileName, std::ios::in);

	if (file.good()) {
		// jeœli istnieje poprzednia matryca, czyœcimy
		if (mat.size()) mat.clear();

		int dimension = 0, cross = 0, valueInMatrix = 0;
		std::string stringTemp;

		file >> dimension;

		this->size = dimension;
		this->mat.reserve(dimension);
		std::vector<std::vector<int>>::iterator matIter = mat.begin();

		for (int i = 0; i < dimension; i++) {
			for (int j = 0; j < dimension; j++) {
				file >> valueInMatrix;

				// pierwszy wiersz wymaga specjalnego traktowania - musimy zrobiæ push_back vectorów
				if (mat.size() < size) {
					std::vector<int> tempVec;
					tempVec.push_back(!mat.size() ? 0 : valueInMatrix);
					mat.push_back(tempVec);

					matIter = mat.begin();
				}
				// reszta normalny pushback
				else {
					if (i == j) (*matIter).push_back(0);
					else (*matIter).push_back(valueInMatrix);

					matIter++;
					if (matIter == mat.end()) matIter = mat.begin();
				}
			}
		}

		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void Matrix::loadFromATSP(std::string fileName) {
	std::fstream file;
	file.open(fileName, std::ios::in);

	if (file.good()) {
		// jeœli istnieje poprzednia matryca, czyœcimy
		if (mat.size()) mat.clear();

		int dimension = 0, cross = 0, valueInMatrix = 0;
		std::string stringTemp;

		do {
			file >> stringTemp;
		} while (stringTemp != "DIMENSION:");

		// Znalezienie wielkoœci matrycy
		std::cout << stringTemp << std::endl;
		file >> dimension;

		this->size = dimension;
		this->mat.reserve(dimension);
		std::vector<std::vector<int>>::iterator matIter = mat.begin();

		std::cout << dimension << std::endl << std::endl;

		do {
			file >> stringTemp;
		} while (stringTemp != "EDGE_WEIGHT_SECTION");

		// Dotarcie do wartoœci zapisanych w matrycy, podgl¹d wartoœci zapisanych na przek¹tnych (?)
		file >> stringTemp;
		cross = std::stoi(stringTemp);

		while (stringTemp != "EOF") {
			valueInMatrix = std::stoi(stringTemp);

			// pierwszy wiersz wymaga specjalnego traktowania - musimy zrobiæ push_back vectorów
			if (mat.size() < size) {
				std::vector<int> tempVec;
				tempVec.push_back(!mat.size() ? 0 : valueInMatrix);
				mat.push_back(tempVec);

				matIter = mat.begin();
			}
			// reszta normalny pushback
			else {
				if (valueInMatrix == cross || valueInMatrix == 0) (*matIter).push_back(0);
				else (*matIter).push_back(valueInMatrix);

				matIter++;
				if (matIter == mat.end()) matIter = mat.begin();
			}

			file >> stringTemp;
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void Matrix::oldLoadFromFile(std::string fileName) {
	std::fstream file;
	file.open(fileName, std::ios::in);

	if (file.good()) {
		// jeœli istnieje poprzednia matryca, czyœcimy
		if (mat.size()) mat.clear();

		int dimension = 0, cross = 0, valueInMatrix = 0;
		std::string stringTemp;

		do {
			file >> stringTemp;
		} while (stringTemp != "DIMENSION:");

		// Znalezienie wielkoœci matrycy
		std::cout << stringTemp << std::endl;
		file >> dimension;

		this->size = dimension;
		this->mat.reserve(dimension);
		std::vector<std::vector<int>>::iterator matIter = mat.begin();

		std::cout << dimension << std::endl << std::endl;

		do {
			file >> stringTemp;
		} while (stringTemp != "EDGE_WEIGHT_SECTION");

		// Dotarcie do wartoœci zapisanych w matrycy, podgl¹d wartoœci zapisanych na przek¹tnych (?)
		file >> stringTemp;
		cross = std::stoi(stringTemp);

		while (stringTemp != "EOF") {
			valueInMatrix = std::stoi(stringTemp);

			// pierwszy wiersz wymaga specjalnego traktowania - musimy zrobiæ push_back vectorów
			if (mat.size() < size) {
				std::vector<int> tempVec;
				tempVec.push_back(!mat.size() ? 0 : valueInMatrix);
				mat.push_back(tempVec);

				matIter = mat.begin();
			}
			// reszta normalny pushback
			else {
				if (valueInMatrix == cross || valueInMatrix == 0) (*matIter).push_back(0);
				else (*matIter).push_back(valueInMatrix);

				matIter++;
				if (matIter == mat.end()) matIter = mat.begin();
			}

			file >> stringTemp;
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void Matrix::display() {
	std::vector<std::vector<int>>::iterator matIter;
	std::vector<int>::iterator matIterInner;

	for (int i = 0; i < mat.size(); i++) {
		for (matIter = mat.begin(); matIter != mat.end(); matIter++) {
			matIterInner = (*matIter).begin();
			std::advance(matIterInner, i);

			int spacesToAdd = 2;
			if (*matIterInner) {
				spacesToAdd -= (int)log10((double)(*matIterInner));
			}

			std::cout << (*matIterInner) << " ";
			while (spacesToAdd > 0) {
				std::cout << " ";
				spacesToAdd--;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Algorithms::displayResults() {
	std::cout << "\nDlugosc sciezki: " << pathLength << "\n";
	std::cout << "Kolejnosc wierzcholkow:\n0 ";
	for (auto a : this->vertexOrder) std::cout << a << " ";
	std::cout << "0\nCzas trwania algorytmu: " << executionTime.count() << "s\n";
}

void Algorithms::generateInitialSolution(Matrix* matrix) {
	// Greedy method
	std::vector<int> possibleVertices;
	int matrixSize = matrix->size;
	for (int i = 1; i < matrixSize; i++)
		possibleVertices.push_back(i);

	int currentVertex = 0;

	while (possibleVertices.size()) {
		int value = INT_MAX, lowestIndex = 0;
		// znajdz najkrotsza mozliwa sciezke
		for (int i = 0; i < matrixSize; i++) {
			if (matrix->mat[i][currentVertex] < value 
				&& (std::find(possibleVertices.begin(), possibleVertices.end(),	i) != std::end(possibleVertices))) {
				value = matrix->mat[i][currentVertex];
				lowestIndex = i;
			}
		}

		// dodaj do generowanego rozwiazania
		vertexOrder.push_back(lowestIndex);
		pathLength += value;

		// zmien "obecny" wierzcholek
		currentVertex = lowestIndex;

		// usun z mozliwych do wybrania wierzcholkow
		possibleVertices.erase(
			std::find(possibleVertices.begin(), possibleVertices.end(), lowestIndex)
		);
	}

	pathLength += matrix->mat[0][currentVertex];
}

void clear() {
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#endif
}