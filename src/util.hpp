#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <unordered_map>

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
	// void tabuSearch();
	void simulatedAnnealing(Matrix* matrix);
	void displayResults();

private:
	int pathLength;
	std::vector<short> vertexOrder;
	std::chrono::duration<double> executionTime;

	void generateInitialSolution(Matrix* matrix);
	// SA
	void generateRandomCandidate(Matrix* matrix);
};

void clear();