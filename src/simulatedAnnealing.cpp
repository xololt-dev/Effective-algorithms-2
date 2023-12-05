#include "util.hpp"

#include <iostream>

void Algorithms::simulatedAnnealing(Matrix* matrix) {
	generateInitialSolution(matrix);

	std::vector<short> currentSolutionOrder = vertexOrder, bestSolutionOrder = vertexOrder;
	int currentSolutionLength = pathLength, bestSolutionLength = pathLength;

	generateRandomCandidate(&currentSolutionOrder, currentNeighbourhoodType);
}

std::vector<short> Algorithms::generateRandomCandidate(std::vector<short>* currentOrder, NeighbourhoodType nearType) {
	std::vector<short> returnVector;

	switch (nearType)
	{
	case INVERSE:
		returnVector = inverse(currentOrder);
		break;
	case SWAP:
		returnVector = swap(currentOrder);
		break;
	case INSERT:
		returnVector = insert(currentOrder);
		break;
	case INSERT_SUB:
		returnVector = insertSub(currentOrder);
		break;
	default:
		break;
	}

	return returnVector;
}

std::vector<short> Algorithms::inverse(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	returnVector.reserve(currentOrder->size());

	return returnVector;
}

std::vector<short> Algorithms::swap(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	returnVector.reserve(currentOrder->size());

	return returnVector;
}

std::vector<short> Algorithms::insert(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	returnVector.reserve(currentOrder->size());

	return returnVector;
}

std::vector<short> Algorithms::insertSub(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	returnVector.reserve(currentOrder->size());

	return returnVector;
}