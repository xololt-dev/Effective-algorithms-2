#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <tuple>

enum NeighbourhoodType {
	INVERSE,
	SWAP,
	INSERT,
	INSERT_SUB
};

class Matrix {
public:
	int size = 0;
	// wype³niane wierszami
	std::vector<std::vector<int>> mat;

	void loadFromFile(std::string fileName);
	void oldLoadFromFile(std::string fileName);
	void display();

private:
	void loadFromTxt(std::string fileName);
	void loadFromATSP(std::string fileName);
};

class Algorithms {
public:
	void setStopCriterium(int value);
	void setCoolingConstant(float value);
	void setNeighbourhoodType(NeighbourhoodType type);

	// void tabuSearch();
	void simulatedAnnealing(Matrix* matrix);
	void displayResults();

private:
	int pathLength;
	std::vector<short> vertexOrder;

	std::chrono::duration<double> executionTime;
	float coolingConstant;
	NeighbourhoodType currentNeighbourhoodType;

	std::random_device rd;
	
	std::tuple<int, int> generateRandomTwoPositions(int lowerBound, int higherBound);

	void generateInitialSolution(Matrix* matrix);
	// SA
	std::vector<short> generateRandomCandidate(std::vector<short>* currentOrder, NeighbourhoodType nearType);
	std::vector<short> inverse(std::vector<short>* currentOrder);
	std::vector<short> swap(std::vector<short>* currentOrder);
	std::vector<short> insert(std::vector<short>* currentOrder);
	std::vector<short> insertSub(std::vector<short>* currentOrder);
};

void clear();