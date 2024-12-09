#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Garage {
    int x, y;
} Garage;

int userX, userY;

// Calculate squared distance from the user location
int squaredDistanceFromUser(Garage g) {
    return (g.x - userX) * (g.x - userX) + (g.y - userY) * (g.y - userY);
}

// Decides which two points come first
int compareTo(const Garage* g1, const Garage* g2) {
    int dist1 = squaredDistanceFromUser(*g1);
    int dist2 = squaredDistanceFromUser(*g2);
    
    if (dist1 != dist2) {
        return dist1 - dist2;
    }
    if (g1->x != g2->x) {
        return g1->x - g2->x;
    }
    return g1->y - g2->y;
}

// Sort for smaller sizes
void insertionSort(Garage* garages, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        Garage key = garages[i];
        int j = i - 1;
        while (j >= left && compareTo(&garages[j], &key) > 0) {
            garages[j + 1] = garages[j];
            j--;
        }
        garages[j + 1] = key;
    }
}

void merge(Garage* garages, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Garage* L = (Garage*)malloc(n1 * sizeof(Garage));
    Garage* R = (Garage*)malloc(n2 * sizeof(Garage));

    for (int i = 0; i < n1; i++) {
        L[i] = garages[left + i];
    }
    for (int i = 0; i < n2; i++) {
        R[i] = garages[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareTo(&L[i], &R[j]) <= 0) {
            garages[k++] = L[i++];
        } else {
            garages[k++] = R[j++];
        }
    }
    while (i < n1) {
        garages[k++] = L[i++];
    }
    while (j < n2) {
        garages[k++] = R[j++];
    }

    free(L);
    free(R);
}

void mergeSort(Garage* garages, int left, int right, int t) {
    if (right - left + 1 <= t) {
        insertionSort(garages, left, right);
    } else {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(garages, left, mid, t);
            mergeSort(garages, mid + 1, right, t);
            merge(garages, left, mid, right);
        }
    }
}

// Find a garage by coordinates
int binarySearch(Garage* garages, int n, Garage* target) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = compareTo(&garages[mid], target);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Not found
}

int main() {
    int n, s, t;

    scanf("%d %d %d %d %d", &userX, &userY, &n, &s, &t);

    Garage* garages = (Garage*)malloc(n * sizeof(Garage));
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &garages[i].x, &garages[i].y);
    }

    Garage* queries = (Garage*)malloc(s * sizeof(Garage));
    for (int i = 0; i < s; i++) {
        scanf("%d %d", &queries[i].x, &queries[i].y);
    }

    mergeSort(garages, 0, n - 1, t);

    for (int i = 0; i < n; i++) {
        printf("%d %d\n", garages[i].x, garages[i].y);
    }

    for (int i = 0; i < s; i++) {
        int result = binarySearch(garages, n, &queries[i]);
        if (result != -1) {
            printf("%d %d garage found at position %d in the order\n", queries[i].x, queries[i].y, result + 1);
        } else {
            printf("%d %d no garage found\n", queries[i].x, queries[i].y);
        }
    }

    free(garages);
    free(queries);

    return 0;
}
