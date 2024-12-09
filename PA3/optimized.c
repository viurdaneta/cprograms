#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Structure representing a garage with coordinates and name
typedef struct Garage {
    int x, y;
    char name[21];
} Garage;

Garage *garages[16];  // Array of pointers for up to 16 garages (since n can be at most 8)
int best_permutation[16];  // Stores the best permutation of garage indices

// Structure to store a pair of garages and their calculated distance
typedef struct Pair {
    Garage *g1;
    Garage *g2;
    double distance;
} Pair;

// Calculates the Euclidean distance between two garages
double calculateDistance(Garage *g1, Garage *g2) {
    return sqrt(((g1->x - g2->x) * (g1->x - g2->x)) + ((g1->y - g2->y) * (g1->y - g2->y)));
}

// Calculates the total distance for the current pairing
double totalDistance(int *permutation, int n) {
    double total = 0.0;
    for (int i = 0; i < 2 * n; i += 2) {
        total += calculateDistance(garages[permutation[i]], garages[permutation[i + 1]]);
    }
    return total;
}

// Swaps two elements in an array (used for generating permutations)
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Generates all permutations and finds the best pairing
void findBestPermutation(int *permutation, int l, int r, int n, double *min_distance) {
    if (l == r) {
        double current_distance = totalDistance(permutation, n);
        if (current_distance < *min_distance) {
            *min_distance = current_distance;
            for (int i = 0; i < 2 * n; i++) {
                best_permutation[i] = permutation[i];
            }
        }
    } else {
        for (int i = l; i <= r; i++) {
            swap(&permutation[l], &permutation[i]);
            findBestPermutation(permutation, l + 1, r, n, min_distance);
            swap(&permutation[l], &permutation[i]);  // Backtrack
        }
    }
}

// Compares two pairs of garages based on their names for sorting
int comparePairs(const void *a, const void *b) {
    Pair *pair1 = (Pair *)a;
    Pair *pair2 = (Pair *)b;
    int cmp = strcmp(pair1->g1->name, pair2->g1->name);
    if (cmp == 0) {
        return strcmp(pair1->g2->name, pair2->g2->name);
    }
    return cmp;
}

// Ensures lexicographical order within a pair of garages
void ensureLexicographicalOrder(Pair *pair) {
    if (strcmp(pair->g1->name, pair->g2->name) > 0) {
        Garage *temp = pair->g1;
        pair->g1 = pair->g2;
        pair->g2 = temp;
    }
}

int main(void) {
    int n;
    scanf("%d", &n);

    // Input garages
    for (int i = 0; i < 2 * n; i++) {
        garages[i] = malloc(sizeof(Garage));
        scanf("%d %d %s", &garages[i]->x, &garages[i]->y, garages[i]->name);
    }

    // Generate initial permutation
    int permutation[16];
    for (int i = 0; i < 2 * n; i++) {
        permutation[i] = i;
    }

    double min_distance = 1e9;

    // Generate all permutations and find the best one
    findBestPermutation(permutation, 0, 2 * n - 1, n, &min_distance);

    // Store pairs of garages and their distances
    Pair pairs[8];
    for (int i = 0; i < 2 * n; i += 2) {
        pairs[i / 2].g1 = garages[best_permutation[i]];
        pairs[i / 2].g2 = garages[best_permutation[i + 1]];
        pairs[i / 2].distance = calculateDistance(garages[best_permutation[i]], garages[best_permutation[i + 1]]);
        ensureLexicographicalOrder(&pairs[i / 2]);  // Ensure correct order within each pair
    }

    // Sort pairs by garage names
    qsort(pairs, n, sizeof(Pair), comparePairs);

    // Output the minimum distance
    printf("%.3f\n", min_distance);

    // Output sorted pairs
    for (int i = 0; i < n; i++) {
        printf("(%s, %s, %.3f)\n", pairs[i].g1->name, pairs[i].g2->name, pairs[i].distance);
    }

    // Free allocated memory
    for (int i = 0; i < 2 * n; i++) {
        free(garages[i]);
    }

    return 0;
}
