#include "util.hpp"

#include <iostream>
#include <algorithm>

void Algorithms::simulatedAnnealing(Matrix* matrix, double t_0, int eraLength, int maxNonImproved) {
	std::tuple<std::vector<short>, int> t = generateInitialSolution(matrix);

	std::cout << "\nDlugosc sciezki zachlannej: " << std::get<1>(t) << "\n";
	std::cout << "Kolejnosc wierzcholkow:\n0 ";
	for (auto a : std::get<0>(t)) std::cout << a << " ";
	std::cout << "\n";

	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	int tempEra = eraLength, maxAllowedNonImprovement = maxNonImproved, notImprovedStreak = 0;
	double currentTemp = t_0;
	if (t_0 == 0.0)
		currentTemp = std::get<1>(t) / log(1 + coolingConstant) / matrix->size; // think about adding time
	if (eraLength <= 0)
		tempEra = 5;
	if (maxNonImproved <= 0)
		maxAllowedNonImprovement = 10;

	std::vector<short> currentSolutionOrder = std::get<0>(t), randomCandidateOrder, bestSolutionOrder = std::get<0>(t);
	int currentSolutionLength = std::get<1>(t), bestSolutionLength = std::get<1>(t);

	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start) < maxExecutionTime) {
		// eraLength = max time per set temperature
		int eraLength = 0;

		while (eraLength < tempEra) {
			randomCandidateOrder = generateRandomCandidate(&currentSolutionOrder, currentNeighbourhoodType);

			int candidatePath = calculateCandidate(&randomCandidateOrder, matrix);

			if (changeSolutions(candidatePath, currentSolutionLength, currentTemp)) {
				if (candidatePath < bestSolutionLength) {
					bestSolutionLength = candidatePath;
					bestSolutionOrder = randomCandidateOrder;
					notImprovedStreak = 0;
				}
				else notImprovedStreak++;

				currentSolutionOrder = randomCandidateOrder;
				currentSolutionLength = candidatePath;
				break;
			}
			else notImprovedStreak++;

			if (notImprovedStreak > maxAllowedNonImprovement) {
				t = generateSecondarySolution(matrix);
				currentSolutionOrder = std::get<0>(t);
				currentSolutionLength = std::get<1>(t);
				notImprovedStreak = 0;
				break;
			}

			eraLength++;
		}

		// decrease temp
		currentTemp *= coolingConstant;
	}

	runningTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
	pathLength = bestSolutionLength;
	vertexOrder = bestSolutionOrder;
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
	std::vector<short> returnVector(*currentOrder);
	int vectorSize = currentOrder->size();
	
	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);
	
	std::vector<short>::iterator low = returnVector.begin(), high = returnVector.begin();
	std::advance(low, std::get<0>(t));
	std::advance(high, std::get<1>(t) + 1);
	std::reverse(low, high);
	/*
	returnVector.reserve(vectorSize);

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);

	// inverse inside vector
	std::vector<short>::iterator low = currentOrder->begin(), high = currentOrder->begin();
	if (std::get<0>(t) > 0) {
		std::advance(low, std::get<0>(t));
		returnVector.insert(returnVector.end(), currentOrder->begin(), low++);
		// std::copy(currentOrder->begin(), low++, returnVector.begin());
	}
	std::advance(high, std::get<1>(t) - 1);
	// returnVector.insert(returnVector.end() - 1, high, low);
	std::copy(high, low, returnVector.end());
	returnVector.insert(returnVector.end() - 1, ++high, currentOrder->end());
	// std::copy(++high, currentOrder->end(), returnVector.begin());
	*/
	return returnVector;
}

std::vector<short> Algorithms::swap(std::vector<short>* currentOrder) {
	std::vector<short> returnVector(*currentOrder);
	int vectorSize = currentOrder->size();
	
	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);

	// swap positions
	std::vector<short>::iterator low = returnVector.begin(), high = returnVector.begin();

	std::advance(low, std::get<0>(t));
	std::advance(high, std::get<1>(t));
	std::swap(low, high);

	return returnVector;
}

std::vector<short> Algorithms::insert(std::vector<short>* currentOrder) {
	std::vector<short> returnVector(*currentOrder);
	int vectorSize = currentOrder->size();

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1, false);

	// insert
	if (std::get<0>(t) > std::get<1>(t)) {
		std::rotate(returnVector.rend() - std::get<0>(t) - 1,
			returnVector.rend() - std::get<0>(t),
			returnVector.rend() - std::get<1>(t));
	}		
	else {
		std::rotate(returnVector.begin() + std::get<0>(t),
			returnVector.begin() + std::get<0>(t) + 1,
			returnVector.begin() + std::get<1>(t) + 1);
	}		

	return returnVector;
}

std::vector<short> Algorithms::insertSub(std::vector<short>* currentOrder) {
	std::vector<short> returnVector(*currentOrder);
	int vectorSize = currentOrder->size();

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);
	std::uniform_int_distribution<> distribution(0, vectorSize - 1);
	int indexThree = distribution(gen);

	// insert group
	if (std::get<0>(t) > indexThree) {
		std::rotate(returnVector.rend() - std::get<1>(t) - 1,
			returnVector.rend() - std::get<0>(t),
			returnVector.rend() - indexThree);
	}
	else {
		std::rotate(returnVector.begin() + std::get<0>(t),
			returnVector.begin() + std::get<1>(t) + 1,
			returnVector.begin() + std::min(indexThree + 2, (int)returnVector.size()));
	}

	return returnVector;
}

bool Algorithms::changeSolutions(int candidatePath, int currentPath, double currentTemp) {
	if (candidatePath <= currentPath)
		return true;

	double p = std::exp(-((double)(candidatePath - currentPath) / currentTemp));
	std::uniform_real_distribution<> distribution(0.0, 1.0);
	double r = distribution(gen);

	if (r <= p)
		return true;

	return false;
}

int Algorithms::calculateCandidate(std::vector<short>* candidateOrder, Matrix* matrix) {
	int pathLength = 0, previousVector = 0;
	std::vector<std::vector<int>>* matrixStart = &(matrix->mat);

	for (auto iter = candidateOrder->begin(); iter != candidateOrder->end(); iter++) {
		pathLength += (*matrixStart)[*iter][previousVector];
		previousVector = *iter;
	}

	pathLength += (*matrixStart)[0][previousVector];

	return pathLength;
}