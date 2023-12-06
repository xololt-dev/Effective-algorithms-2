#include "util.hpp"

#include <iostream>

void Algorithms::simulatedAnnealing(Matrix* matrix) {
	generateInitialSolution(matrix);

	std::vector<short> currentSolutionOrder = vertexOrder, randomCandidateOrder, bestSolutionOrder = vertexOrder;
	int currentSolutionLength = pathLength, bestSolutionLength = pathLength;

	randomCandidateOrder = generateRandomCandidate(&currentSolutionOrder, currentNeighbourhoodType);


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
	int vectorSize = currentOrder->size();
	returnVector.reserve(vectorSize);

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);

	// inverse inside vector
	std::vector<short>::iterator low = currentOrder->begin(), high = currentOrder->end();
	if (std::get<0>(t) > 0) {
		std::advance(low, std::get<0>(t) - 1);
		std::copy(currentOrder->begin(), low++, returnVector.begin());
	}
	std::advance(high, std::get<1>(t) - 1);
	std::copy(high, low, returnVector.begin());
	std::copy(++high, currentOrder->end(), returnVector.begin());

	return returnVector;
}

std::vector<short> Algorithms::swap(std::vector<short>* currentOrder) {
	std::vector<short> returnVector(*currentOrder);
	int vectorSize = currentOrder->size();
	
	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);

	// swap positions
	std::vector<short>::iterator low = currentOrder->begin(), high = currentOrder->end();

	std::advance(low, std::get<0>(t));
	std::advance(high, std::get<1>(t));
	std::swap(low, high);

	return returnVector;
}

std::vector<short> Algorithms::insert(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	int vectorSize = currentOrder->size();
	returnVector.reserve(vectorSize);

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);

	return returnVector;
}

std::vector<short> Algorithms::insertSub(std::vector<short>* currentOrder) {
	std::vector<short> returnVector;
	int vectorSize = currentOrder->size();
	returnVector.reserve(vectorSize);

	return returnVector;
}