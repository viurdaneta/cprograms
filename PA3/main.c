/* COP 3502C Assignment 3
This program is written by: Victor Urdaneta */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct Garage {
    int x, y;
    char name[21];
} Garage;

Garage garages[16];  // Array for up to 16 garages (since n can be at most 8)
int best_permutation[16];  // Stores the indices of the garages that give the minimum distance

double calculateDistance(Garage *g1, Garage *g2) {
    return sqrt( ((g1->x - g2->x) * (g1->x - g2->x)) + ((g1->y - g2->y) * (g1->y - g2->y)));
}

double minDistance(Garage **garages, int garage_count) {
    if (garage_count == 0) {
        return 0;
    }
    // Calculate the distance between the first pair
    double distance = calculateDistance(garages[0], garages[1]);
    
    // Recursively calculate the distance for the remaining pairs
    return distance + minDistance(garages + 2, garage_count - 2);
}


int main(void) 
{
    int n;
    scanf("%d", &n);
    Garage *garages[2*n];
    for(int i = 0; i < 2*n; i++) {
        if (garages[i] == NULL) {
            return 0; 
        }
        garages[i] = malloc(sizeof(Garage));
        scanf("%d %d %s", &garages[i]->x, &garages[i]->y, garages[i]->name);
    }
    
    double result = minDistance(garages, 2 * n);
    printf("%.3f\n", result);
    for (int i = 0; i < 2*n; i+=2)
    {
        printf("(%s, %s, %.3f)\n", garages[i]->name, garages[i+1]->name, calculateDistance(garages[i], garages[i+1]));
    }
    
    return 0;
}