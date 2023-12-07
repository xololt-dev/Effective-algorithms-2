#include "util.hpp"

#include <iostream>
#include <algorithm>

void Algorithms::simulatedAnnealing(Matrix* matrix, double t_0, int eraLength, int maxNonImproved) {	
	// greedy path generator
	std::tuple<std::vector<short>, int> t = generateInitialSolution(matrix);
	int secondVertex = std::get<0>(t)[0];
	
	std::cout << "\nDlugosc sciezki zachlannej: " << std::get<1>(t) << "\n";
	std::cout << "Kolejnosc wierzcholkow:\n0 ";
	for (auto a : std::get<0>(t)) std::cout << a << " ";
	std::cout << "0\n";

	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	/* 
	* pathDelta:
	* - low value
	*	- not much difference path to path
	*	- suggestion: set high discovery, low-ish local search 
	* - high value
	*	- big difference in total path lengths
	*	- suggestion: set low discovery, high local search
	* tempEra:
	* - low value
	*	- checks neighbourhoods for a short time
	*	- higher discoverity
	* - high value
	*	- checks neighbourhoods for a long time
	*	- higher local search
	* maxAllowedNonImprovement:
	* - low value
	*	- re-generates current solution after short time 
	*	- higher discoverity
	* - high value
	*	- re-generates current solution after long time 
	*	- higher local search
	* currentTemp:
	* - also "temperature zero" at the beginning
	* - low value
	*	- rarely takes worse solution
	*	- higher local search
	* - high value
	*	- frequently takes worse solution
	*	- higher discoverity
	*/
	int pathDelta = getPathDelta(matrix);
	int tempEra = eraLength, maxAllowedNonImprovement = maxNonImproved, notImprovedStreak = 0;
	double currentTemp = t_0;

	if (t_0 == 0.0)
		currentTemp = std::get<1>(t) / log(1 + coolingConstant) / (double) pathDelta; // / sqrt(matrix->size); 
		//currentTemp = std::get<1>(t) / log(1 + coolingConstant) / matrix->size; // think about adding time
	if (eraLength <= 0)
		tempEra = std::min(pathDelta, matrix->size - 2);
	if (maxNonImproved <= 0)
		maxAllowedNonImprovement = pathDelta * pathDelta * pathDelta / 2;

	std::cout << "\nT_0: " << currentTemp << "\n";
	std::cout << "Cooling coefficient: " << coolingConstant << "\n";
	std::cout << "Path delta: " << pathDelta << "\n";
	std::cout << "Era length: " << tempEra << "\n";
	std::cout << "Max non improvement streak allowed: " << maxAllowedNonImprovement << "\n";

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

					runningTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
				}
				else notImprovedStreak++;

				currentSolutionOrder = randomCandidateOrder;
				currentSolutionLength = candidatePath;
			}
			else notImprovedStreak++;

			if (notImprovedStreak > maxAllowedNonImprovement) {
				t = generateThirdSolution(matrix, secondVertex);
				currentSolutionOrder = std::get<0>(t);
				currentSolutionLength = std::get<1>(t);
				secondVertex = std::get<0>(t)[0];
				notImprovedStreak = 0;
				break;
			}

			eraLength++;
		}

		// decrease temp
		currentTemp *= coolingConstant;
	}

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
	std::vector<short>::iterator beginCurrent = currentOrder->begin();

	// generate two positions
	std::tuple<int, int> t = generateRandomTwoPositions(0, vectorSize - 1);
	std::uniform_int_distribution<> distribution(0, vectorSize - 1);
	int indexThree = distribution(gen);

	
	/*
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
	*/

	// insert group
	if (indexThree > std::get<1>(t)) {
		if (std::get<0>(t) == 0) {
			returnVector.insert(returnVector.begin(), beginCurrent + std::get<1>(t) + 1, beginCurrent + indexThree + 1);
			returnVector.insert(returnVector.begin() + (indexThree - std::get<1>(t)), beginCurrent + std::get<0>(t), beginCurrent + std::get<1>(t) + 1);
		}
		else {
			returnVector.insert(returnVector.begin(), beginCurrent, beginCurrent + std::get<0>(t));
			returnVector.insert(returnVector.end(), beginCurrent + std::get<1>(t) + 1, beginCurrent + indexThree + 1);
			returnVector.insert(returnVector.end(), beginCurrent + std::get<0>(t), beginCurrent + std::get<1>(t) + 1);
		}
	}
	else if (indexThree < std::get<0>(t)) {
		returnVector.insert(returnVector.begin(), beginCurrent, beginCurrent + indexThree);
		returnVector.insert(returnVector.end(), beginCurrent + std::get<0>(t), beginCurrent + std::get<1>(t) + 1);
		returnVector.insert(returnVector.end(), beginCurrent + indexThree, beginCurrent + std::get<0>(t));
		returnVector.insert(returnVector.end(), beginCurrent + std::get<1>(t) + 1, currentOrder->end());
	}
	else {
		returnVector.insert(returnVector.begin(), beginCurrent, beginCurrent + std::get<0>(t));
		returnVector.insert(returnVector.end(), beginCurrent + std::get<1>(t) + 1, beginCurrent + std::min(std::get<1>(t) + (std::get<1>(t) - std::get<0>(t)), (int) currentOrder->size()));
		returnVector.insert(returnVector.end(), beginCurrent + std::get<0>(t), beginCurrent + std::get<1>(t) + 1);
	}

	return returnVector;
}

int Algorithms::getPathDelta(Matrix* matrix) {
	int lowest = INT_MAX, highest = 0;

	for (std::vector<std::vector<int>>::iterator i = matrix->mat.begin(); i != matrix->mat.end(); i++) {
		for (auto j = (*i).begin(); j != (*i).end(); j++) {
			if (*j < lowest)
				lowest = *j;
			else if (*j > highest)
				highest = *j;
		}
	}

	return highest - lowest;
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