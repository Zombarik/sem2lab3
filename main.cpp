#include <iostream>
#include <fstream>
#include <time.h>
#include "DijkstraCalculator.cpp"

using namespace std;

void multithreadingTest(DijkstraCalculator& calculator, int maxThreadsNum) {
	string OUTPUT_FILE_NAME = "executionTimeResults.txt";

	cout << "Input generated, processing..." << endl;

	ofstream RESULTS_FILE(OUTPUT_FILE_NAME);
	RESULTS_FILE << "Number of threads - " << maxThreadsNum << endl;

	for (int threadCount = 1; threadCount <= maxThreadsNum; threadCount++) {
		RESULTS_FILE << threadCount << " " << calculator.run(threadCount) << endl;
	}

	RESULTS_FILE.flush();
}


int** loadFromFile(string inputFilePath, int& nodeNum) {

	ifstream inputFile(inputFilePath);
	inputFile >> nodeNum;

	int** edges = (int**)malloc(sizeof(int*) * nodeNum);
	for (int i = 0; i < nodeNum; i++) {
		edges[i] = (int*)malloc(sizeof(int) * nodeNum);
	}

	for (int i = 0; i < nodeNum; i++) {
		for (int j = 0; j < nodeNum; j++) {
			inputFile >> edges[i][j];
		}
	}

	return edges;
}



int main(int argc, char* argv[]) {

	switch (atoi(argv[2])) {

	case 1: {
		int maxThreadsNum = atoi(argv[1]);
		string inputFilePath = argv[3];
		int startNode = atoi(argv[4]);
		string outputFilePath = argv[5];
		int nodeNum = 0;

		int** matrix = loadFromFile(inputFilePath, nodeNum);

		DijkstraCalculator calculator = DijkstraCalculator(nodeNum, startNode, matrix);

		multithreadingTest(calculator, maxThreadsNum);

		calculator.saveResult(outputFilePath);
		break;
	}

	case 2: {
		int maxThreadsNum = atoi(argv[1]);
		int nodeNum = atoi(argv[3]);
		int startNode = atoi(argv[4]);
		string outputFilePath = argv[5];

		Generator generator = Generator(nodeNum);
		DijkstraCalculator calculator = DijkstraCalculator(nodeNum, startNode, generator.generateGraph());

		multithreadingTest(calculator, maxThreadsNum);

		calculator.saveResult(outputFilePath);

		break;
	}
	}

	return 0;
}


