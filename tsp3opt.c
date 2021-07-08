#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

// Ozan Yerli

struct city {
	int id, x, y, visited;
};

int calcDistance(int x1, int y1, int x2, int y2) { // Calculates and returns the euclidean distance between two cities

	double distance = sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
	distance = round(distance);
	int result = (int) distance;
	return result;
}

int nearestNeighbor(struct city **cities, int id, void *dist, int n) { // // Returns the id of the nearest neighbor to the given city

	int (*distances)[n] = dist;
	int minDistance = INT_MAX;
	int neighborID = -1;
	int i;
	for (i = 0; i < n; i++) {

		if (distances[id][i] < minDistance && i != id && !cities[i]->visited) {
			minDistance = distances[id][i];
			neighborID = i;
		}
	}
	return neighborID;
}

int bestMove(void *dist, int a, int b, int c, int d, int e, int f, int n, int *totDistance) { // Finds the case that results in maximum length gain

	int (*distances)[n] = dist;

	int gains[8];

	gains[0] = 0;

	gains[1] = distances[a][e] + distances[b][f] - distances[a][b] - distances[e][f];

	gains[2] = distances[c][e] + distances[d][f] - distances[c][d] - distances[e][f];

	gains[3] = distances[a][c] + distances[b][d] - distances[a][b] - distances[c][d];

	int deletedEdges = distances[a][b] + distances[c][d] + distances[e][f];

	gains[4] = distances[a][c] + distances[b][e] + distances[d][f] - deletedEdges;

	gains[5] = distances[a][e] + distances[d][b] + distances[c][f] - deletedEdges;

	gains[6] = distances[a][d] + distances[e][c] + distances[b][f] - deletedEdges;

	gains[7] = distances[a][d] + distances[e][b] + distances[c][f] - deletedEdges;

	int maxGain = 0;
	int bestCase = 0;
	int i;
	for (i = 1; i < 8; i++) {
		if (gains[i] < 0 && gains[i] < maxGain) {
			bestCase = i;
			maxGain = gains[i];
		}
	}

	*totDistance = *totDistance + maxGain;

	return bestCase;
}

void reverse(int path[], int start, int end, int n) { // Reverses a segment between two cities

	int half = ((n + end - start + 1) % n) / 2;

	int left = start;
	int right = end;
	int temp;

	int i;
	for (i = 1; i < half + 1; i++) {
		temp = path[right];
		path[right] = path[left];
		path[left] = temp;

		left = (left + 1) % n;
		right = (n + right - 1) % n;
	}
}

void makeMove(int path[], int bestCase, int i, int j, int k, int n) { // Changes the tour with reversing the segments, depending on the case

	if (bestCase == 1) {

		reverse(path, (k + 1) % n, i, n);

	} else if (bestCase == 2) {

		reverse(path, (j + 1) % n, k, n);

	} else if (bestCase == 3) {

		reverse(path, (i + 1) % n, j, n);

	} else if (bestCase == 4) {

		reverse(path, (j + 1) % n, k, n);
		reverse(path, (i + 1) % n, j, n);

	} else if (bestCase == 5) {

		reverse(path, (k + 1) % n, i, n);
		reverse(path, (i + 1) % n, j, n);

	} else if (bestCase == 6) {

		reverse(path, (k + 1) % n, i, n);
		reverse(path, (j + 1) % n, k, n);

	} else if (bestCase == 7) {

		reverse(path, (k + 1) % n, i, n);
		reverse(path, (i + 1) % n, j, n);
		reverse(path, (j + 1) % n, k, n);

	}
}

int main() {

	FILE *fPtr;

	fPtr = fopen("test-input-1.txt", "r");

	if (fPtr == NULL) {
		printf("Input not found");
		exit(0);
	}

	int id, x, y;

	// Determining the size of input
	while (1) {

		fscanf(fPtr, "%d %d %d\n", &id, &x, &y);

		if (feof(fPtr))
			break;

	}

	int n = id + 1;
	printf("size = %d\n", n);

	struct city *cities[n];

	rewind(fPtr);

	struct city *newCity;

	// Reading input
	while (1) {

		fscanf(fPtr, "%d %d %d\n", &id, &x, &y);
		//printf("%d %d %d\n", id, x, y);

		newCity = malloc(sizeof(struct city));
		newCity->id = id;
		newCity->x = x;
		newCity->y = y;
		newCity->visited = 0;

		cities[id] = newCity;

		if (feof(fPtr))
			break;

	}


	int i, j;

	// Filling the adjacency matrix, holding the distance between each city
	int (*distances)[n] = malloc(sizeof(int[n][n]));

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			distances[i][j] = calcDistance(cities[i]->x, cities[i]->y, cities[j]->x, cities[j]->y);
		}
	}

	int path[n];

	// Starting at a random city
	srand(time(NULL));
	int start = rand() % n;
	//int start = 827;
	cities[start]->visited = 1;
	path[0] = start;

	printf("Starting at %d\n", start);

	id = start;
	int counter = 1;
	int totDistance = 0;

	// Finding a tour with the nearest-neighbor algorithm
	while (1) {
		path[counter] = nearestNeighbor(cities, id, distances, n);
		totDistance += distances[id][path[counter]];
		id = path[counter];
		cities[id]->visited = 1;
		if (counter == n - 1)
			break;
		counter++;
	}

	free(cities);
	free(newCity);

	totDistance += distances[path[n - 1]][path[0]];

	/*printf("Path with nearest neighbor: ");
	for (counter = 0; counter < n; counter++)
	printf("%d ", path[counter]); */

	printf("NN total distance = %d", totDistance);

	int k, a, b, c, d, e, f, improves = 1;

	int bestCase;

	// Improving the tour with 3-opt algorithm
	while (improves) { // While the tour improves
		improves = 0;
		int c1, c2, c3;
		loops:
		for (c1 = 0; c1 < n; c1++) { // For each three edges
			i = c1;
			a = path[i];
			b = path[(i + 1) % n];

			for (c2 = 1; c2 < n - 2; c2++) {
				j = (i + c2) % n;
				c = path[j];
				d = path[(j + 1) % n];
				for (c3 = c2 + 1; c3 < n; c3++) {
					k = (i + c3) % n;
					e = path[k];
					f = path[(k + 1) % n];

					bestCase = bestMove(distances, a, b, c, d, e, f, n, &totDistance); // Calculate the gain for each case
					if (bestCase != 0) { // Make the move if a shorter tour is possible
						makeMove(path, bestCase, i, j, k, n);
						printf("distance = %d\n", totDistance);
						improves = 1;
						goto loops; // Break the loop
					}
				}
			}
		}
	}

	free(distances);

	// Writing to output
	fPtr = fopen("output.txt", "w");
	fprintf(fPtr, "%d\n", totDistance);

	for (i = 0; i < n; i++) {
		fprintf(fPtr, "%d\n", path[i]);
	}

	fclose(fPtr);

	printf("3opt total distance = %d", totDistance);

	return 0;
}
