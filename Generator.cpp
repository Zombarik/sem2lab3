#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

using namespace std;

#define GENEDGE_HAS_PATH_TO_START 0
#define GENEDGE_NO_PATH_TO_START -1
#define MAXIMUM_WEIGHT 100
#define NO_EDGE_BEWTEEN -3

class Generator {

	private:
		const string OUTPUT_FILE_NAME = "input_matrix.txt";

		int nodeNum;
		int** edges;

		void generateEdges() {
			cout << "Generating input matrix..." << endl;
			int counter = 0;
			for (int i = 1; i < nodeNum; i++) {
				int nodeIndex = rand() % i;                 
				int weight = (rand() % (MAXIMUM_WEIGHT - 10)) + 10;
				edges[i][nodeIndex] = weight;
				edges[nodeIndex][i] = weight;
				++counter;
			}

			for (int i = 0; i < nodeNum; i++) {
				for (int j = i; j < nodeNum; j++) {
					if (i == j) {
						continue;
					}

					int weight = (rand() % (MAXIMUM_WEIGHT - 10)) + 10;
					edges[i][j] = edges[j][i] = weight;
					++counter;
				}
			}

			cout << "Total edges: " << counter << endl;
		}
	
		void saveToFile() {
			ofstream file_out(OUTPUT_FILE_NAME, ofstream::trunc);
			file_out << nodeNum << "\n";

			for (int i = 0; i < nodeNum; i++) {
				for (int j = 0; j < nodeNum; j++) {
					if (edges[i][j] == NO_EDGE_BEWTEEN) {
						file_out << setw(2) << "-" << " ";
					} else {
						file_out << setw(2) << edges[i][j] << " ";
					}
				}
				file_out << "\n";
			}
		
			file_out.flush();
		}

		int** createEmptyMatrix() {
			int**  edges = (int**)malloc(sizeof(int*) * nodeNum);

			for (int i = 0; i < nodeNum; i++) {
				edges[i] = (int*)malloc(sizeof(int) * nodeNum);
			}

			for (int i = 0; i < nodeNum; i++) {
				for (int j = 0; j < nodeNum; j++) {
					edges[i][j] = NO_EDGE_BEWTEEN;
				}
			}

			return edges;
		}

		void printMatrix() {
			for (int i = 0; i < nodeNum; i++) {
				for (int j = 0; j < nodeNum; j++) {
					if (edges[i][j] == NO_EDGE_BEWTEEN) {
						cout << setw(2) << "-" << " ";
					} else {
						cout << setw(2) << edges[i][j] << " ";
					}
				}
				cout << "\n";
			}
		}

		void freeMemory() {
			for (int i = 0; i < nodeNum; i++) {
				free(edges[i]);
			}

			free(edges);
		}

	public:
		Generator(int nodeNum) {
			this->nodeNum = nodeNum;
			this->edges = createEmptyMatrix();
		}

		~Generator() {
			freeMemory();
		}

		int** generateGraph() {
			srand(time(NULL));

			generateEdges();
			//saveToFile();
			//printMatrix();

			return edges;
		}
};