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
	else
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

		// Znalezienie wielkoœci matrycy
		do {
			file >> stringTemp;
		} while (stringTemp != "DIMENSION:");

		file >> dimension;

		this->size = dimension;
		this->mat.reserve(dimension);
		std::vector<std::vector<int>>::iterator matIter = mat.begin();

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
	std::cout << "0\nCzas trwania algorytmu: " << runningTime.count() << "s\n";
}

void Algorithms::setStopCriterium(int value) {
	maxExecutionTime = std::chrono::seconds(value);
}

void Algorithms::setCoolingConstant(float value) {
	coolingConstant = value;
}

void Algorithms::setNeighbourhoodType(NeighbourhoodType type) {
	currentNeighbourhoodType = type;
}

void Algorithms::initRandom() {
	gen.seed(rd());
}

std::tuple<int, int> Algorithms::generateRandomTwoPositions(int lowerBound, int higherBound, bool correctOrder) {
	// generate two positions
	std::uniform_int_distribution<> distribution(lowerBound, higherBound);

	int indexOne = distribution(gen), indexTwo = distribution(gen);

	while (indexOne == indexTwo)
		indexTwo = distribution(gen);

	if (!correctOrder) 
		return std::make_tuple(indexOne, indexTwo);

	if (indexTwo < indexOne) {
		int temp = indexOne;
		indexOne = indexTwo;
		indexTwo = temp;
	}
	
	return std::make_tuple(indexOne, indexTwo);
}

std::tuple<std::vector<short>, int> Algorithms::generateInitialSolution(Matrix* matrix) {
	// Greedy method
	std::vector<short> possibleVertices, returnVector;
	int matrixSize = matrix->size, returnLength = 0;
	returnVector.reserve(matrixSize);
	for (int i = 1; i < matrixSize; i++)
		possibleVertices.push_back(i);

	int currentVertex = 0;

	while (possibleVertices.size()) {
		int value = INT_MAX, lowestIndex = 0;
		// znajdz najkrotsza mozliwa sciezke
		for (int i = 1; i < matrixSize; i++) {
			if (matrix->mat[i][currentVertex] < value 
				&& (std::find(possibleVertices.begin(), possibleVertices.end(),	i) != std::end(possibleVertices))) {
				value = matrix->mat[i][currentVertex];
				lowestIndex = i;
			}
		}

		// dodaj do generowanego rozwiazania
		returnVector.push_back(lowestIndex);
		returnLength += value;

		// zmien "obecny" wierzcholek
		currentVertex = lowestIndex;

		// usun z mozliwych do wybrania wierzcholkow
		possibleVertices.erase(
			std::find(possibleVertices.begin(), possibleVertices.end(), lowestIndex)
		);
	}

	returnLength += matrix->mat[0][currentVertex];

	return std::make_tuple(returnVector, returnLength);
}

std::tuple<std::vector<short>, int> Algorithms::generateSecondarySolution(Matrix* matrix) {
	// Random method
	std::vector<short> possibleVertices, returnVector;
	int matrixSize = matrix->size, returnLength = 0;
	returnVector.reserve(matrixSize);
	for (int i = 1; i < matrixSize; i++)
		possibleVertices.push_back(i);

	int currentVertex = 0;
	while (possibleVertices.size()) {
		int value = INT_MAX, lowestIndex = 0;

		// wylosuj nastepny wierzcholek
		std::uniform_int_distribution<> distribution(0, possibleVertices.size() - 1);
		int randomVertex = distribution(gen);
		
		// dodaj do generowanego rozwiazania
		returnVector.push_back(possibleVertices[randomVertex]);
		returnLength += matrix->mat[possibleVertices[randomVertex]][currentVertex];
		
		// zmien "obecny" wierzcholek
		currentVertex = possibleVertices[randomVertex];

		// usun z mozliwych do wybrania wierzcholkow
		possibleVertices.erase(
			std::find(possibleVertices.begin(), possibleVertices.end(), currentVertex)
		);
	}

	returnLength += matrix->mat[0][currentVertex];

	return std::make_tuple(returnVector, returnLength);
}

std::tuple<std::vector<short>, int> Algorithms::generateThirdSolution(Matrix* matrix, int notAllowedSecondary) {
	// Greedy method
	std::vector<short> possibleVertices, returnVector;
	int matrixSize = matrix->size, returnLength = INT_MAX;
	returnVector.reserve(matrixSize);
	for (int i = 1; i < matrixSize; i++)
		possibleVertices.push_back(i);	

	std::uniform_int_distribution<> u(1, matrixSize - 1);

	// zmien "obecny" wierzcholek
	int currentVertex = notAllowedSecondary;
	do {
		currentVertex = u(rd);
	} while (currentVertex == notAllowedSecondary);
	returnVector.push_back(currentVertex);
	returnLength = matrix->mat[currentVertex][0];

	// usun z mozliwych do wybrania wierzcholkow
	possibleVertices.erase(
		std::find(possibleVertices.begin(), possibleVertices.end(), currentVertex)
	);

	while (possibleVertices.size()) {
		int value = INT_MAX, lowestIndex = 0;
		// znajdz najkrotsza mozliwa sciezke
		for (int i = 1; i < matrixSize; i++) {
			if (matrix->mat[i][currentVertex] < value
				&& (std::find(possibleVertices.begin(), possibleVertices.end(), i) != std::end(possibleVertices))) {
				value = matrix->mat[i][currentVertex];
				lowestIndex = i;
			}
		}

		// dodaj do generowanego rozwiazania
		returnVector.push_back(lowestIndex);
		returnLength += value;

		// zmien "obecny" wierzcholek
		currentVertex = lowestIndex;

		// usun z mozliwych do wybrania wierzcholkow
		possibleVertices.erase(
			std::find(possibleVertices.begin(), possibleVertices.end(), lowestIndex)
		);
	}

	returnLength += matrix->mat[0][currentVertex];

	return std::make_tuple(returnVector, returnLength);
}

void clear() {
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#endif
}