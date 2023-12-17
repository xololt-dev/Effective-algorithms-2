#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <queue>
#include <random>
#include <list>
#include <numeric>

std::vector<short> generateNeighbor(const std::vector<short>& solution) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::vector<short> neighbor = solution;
	int n = neighbor.size();
	std::uniform_int_distribution<int> dist(1, n - 1);  // Pomijamy pierwsze miasto (zak³adamy, ¿e jest to punkt startowy)
	int i = dist(g);
	int j = dist(g);
	while (i == j) {
		j = dist(g);
	}
	std::swap(neighbor[i], neighbor[j]);
	return neighbor;
}

// Obliczanie kosztu danej œcie¿ki
int calculatePathCost(const std::vector<short>& path, Matrix* matrix) {
	int totalCost = 0;
	for (size_t i = 0; i < path.size() - 1; ++i) {
		totalCost += matrix->mat[path[i]][path[i + 1]];
	}
	totalCost += matrix->mat[path.back()][path[0]];
	return totalCost;
}

// Implementacja algorytmu Tabu Search
void Algorithms::tabuSearch(Matrix* matrix) {
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	std::vector<short> currentSolution(matrix->size);
	std::iota(currentSolution.begin(), currentSolution.end(), 0);  // Inicjalizacja rozwi¹zania pocz¹tkowego
	std::shuffle(currentSolution.begin() + 1, currentSolution.end(), std::mt19937(std::random_device()()));

	int bestCost = calculatePathCost(currentSolution, matrix);
	std::vector<short> bestSolution = currentSolution;

	std::list<std::vector<short>> tabuList;  // Lista tabu
	const int maxTabuSize = 10;  // Maksymalny rozmiar listy tabu
	const int numIterations = 1000;  // Liczba iteracji

	for (int iteration = 0; iteration < numIterations; ++iteration) {
		auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start); //czas wykonywania
		std::vector<short> neighbor = generateNeighbor(currentSolution);
		int neighborCost = calculatePathCost(neighbor, matrix);

		if (std::find(tabuList.begin(), tabuList.end(), neighbor) == tabuList.end() && neighborCost < bestCost) {
			bestSolution = neighbor;
			bestCost = neighborCost;

			// Aktualizacja listy tabu
			if (tabuList.size() == maxTabuSize) {
				tabuList.pop_front();
			}
			tabuList.push_back(currentSolution);

			currentSolution = neighbor;
		}
	}

	this->pathLength = bestCost;
	this->vertexOrder = bestSolution;
	std::reverse(this->vertexOrder.begin(), this->vertexOrder.end());
	this->vertexOrder.pop_back();
}