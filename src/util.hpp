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
	void initRandom();

	void tabuSearch(Matrix* matrix);
	void simulatedAnnealing(Matrix* matrix, double t_0 = 0.0, int eraLength = 0, int maxNonImproved = 0);
	void benchmarkAnnealing(Matrix* matrix);
	void displayResults();

private:
	int pathLength;
	std::vector<short> vertexOrder;

	std::chrono::duration<double> maxExecutionTime = std::chrono::seconds(30);
	std::chrono::duration<double> runningTime;
	double coolingConstant = 0.99f;
	NeighbourhoodType currentNeighbourhoodType = INVERSE;

	std::random_device rd;
	std::mt19937 gen;
	std::tuple<int, int> generateRandomTwoPositions(int lowerBound, int higherBound, bool correctOrder = 1);

	std::tuple<std::vector<short>, int> generateInitialSolution(Matrix* matrix);
	std::tuple<std::vector<short>, int> generateSecondarySolution(Matrix* matrix);
	std::tuple<std::vector<short>, int> generateThirdSolution(Matrix* matrix, int notAllowedSecondary);
	int getPathDelta(Matrix* matrix);
	// SA
	std::vector<short> generateRandomCandidate(std::vector<short>* currentOrder, NeighbourhoodType nearType);
	std::vector<short> inverse(std::vector<short>* currentOrder);
	std::vector<short> swap(std::vector<short>* currentOrder);
	std::vector<short> insert(std::vector<short>* currentOrder);
	std::vector<short> insertSub(std::vector<short>* currentOrder);

	bool changeSolutions(int candidatePath, int currentPath, double currentTemp);
	int calculateCandidate(std::vector<short>* candidateOrder, Matrix* matrix);
};

void clear();