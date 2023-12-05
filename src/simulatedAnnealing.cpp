#include "util.hpp"

#include <iostream>

void Algorithms::simulatedAnnealing(Matrix* matrix) {
	generateInitialSolution(matrix);

	std::vector<short> currentSolutionOrder = vertexOrder, bestSolutionOrder = vertexOrder;
	int currentSolutionLength = pathLength, bestSolutionLength = pathLength;


}