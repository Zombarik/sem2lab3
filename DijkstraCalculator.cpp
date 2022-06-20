#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include <utility>
#include <chrono> 
#include <algorithm>
#include "Generator.cpp"


using namespace std;
using namespace std::chrono;

#define UNKNOWN_DISTANCE -1
#define NO_EDGE_BEWTEEN -3
#define UNKNOWN_NODE -2
#define NODE_VISITED 1
#define NODE_UNVISITED 0


class DijkstraCalculator {
	private:
		int nodeNum;
		int startNode;
		int** edges;
		int* distanceFromStart;
		int* visitedNodes;

		int findNodeToVisit() {
			int closestNode = -1;
			int closestDistanceGlobal = INT32_MAX;

			int myClosestNode;
			int myClosestDistance;

			#pragma omp parallel private(myClosestNode, myClosestDistance)
			{
				myClosestNode = closestNode;
				myClosestDistance = closestDistanceGlobal;

				#pragma omp for nowait  
				for (int i = 0; i < nodeNum; i++) {
					if ((visitedNodes[i] == NODE_UNVISITED) && (distanceFromStart[i] != UNKNOWN_DISTANCE) && (distanceFromStart[i] < myClosestDistance)) {
						myClosestNode = i;
						myClosestDistance = distanceFromStart[i];
					}
				}

				#pragma omp critical 
				{
					if (myClosestDistance < closestDistanceGlobal) {
						closestDistanceGlobal = myClosestDistance;
						closestNode = myClosestNode;
					}
				}
			}

			return closestNode;
		}

		void visitNode(int currentNode) {
			int currentDistanceFromStart = distanceFromStart[currentNode];

			#pragma omp parallel for schedule(static)   
			for (int i = 0; i < nodeNum; i++) {
				if ((visitedNodes[i] == NODE_VISITED) || (i == currentNode)) {
					continue;
				}

				if (edges[currentNode][i] != NO_EDGE_BEWTEEN) {
					if ((distanceFromStart[i] == UNKNOWN_DISTANCE) || (distanceFromStart[i] > (currentDistanceFromStart + edges[currentNode][i]))) {
						distanceFromStart[i] = (currentDistanceFromStart + edges[currentNode][i]);
					}
				}

			}
		}

		void dijkstraCompute() {
			for (int i = 0; i < nodeNum; i++) {
				int currentNode = findNodeToVisit();
				visitNode(currentNode);
				visitedNodes[currentNode] = NODE_VISITED;
			}
		}

		void setUp() {
			for (int i = 0; i < nodeNum; i++) {
				visitedNodes[i] = NODE_UNVISITED;
				distanceFromStart[i] = UNKNOWN_DISTANCE;
			}

			distanceFromStart[startNode] = 0;
		}


	public:
		DijkstraCalculator(int nodeNum, int startNode, int** edges) {
			this->nodeNum = nodeNum;
			this->edges = edges;
			this->startNode = startNode;

			this->distanceFromStart = (int*)malloc(sizeof(int) * nodeNum);
			this->visitedNodes = (int*)malloc(sizeof(int) * nodeNum);
		}

		~DijkstraCalculator() {
			free(distanceFromStart);
		}

		void saveResult(string outputPath) {
			ofstream file_out(outputPath);
			file_out << nodeNum << " " << startNode << "\n";
			for (int i = 0; i < nodeNum; i++) {
				file_out << distanceFromStart[i] << " ";
			}
			file_out.flush();
		}


		auto run(int num_threads) {
			omp_set_dynamic(0);
			omp_set_num_threads(num_threads);
			
			setUp();
			auto start = high_resolution_clock::now();
			dijkstraCompute();
			auto end = high_resolution_clock::now();

			auto duration1 = duration_cast<microseconds>(end - start);
			auto duration2 = duration_cast<milliseconds>(end - start);
			cout << "Number of threads: " << num_threads << " -> Time taken by function: " << duration1.count() << " microseconds (" << duration_cast<milliseconds>(end - start).count() << " milliseconds)" << endl;

			return duration1.count();
		}
};
