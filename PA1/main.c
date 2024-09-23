/* COP 3502C Assignment 1
This program is written by:  Victor Urdaneta */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct RegisteredVehicle{ //for one registered vehicle
    char* license_plate; //to be used for string license plate# of the vehicle
    char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage{ //for one garage
    char* garage_name; //to be used for garage name
    int total_capacity; //total capacity of the garage
    int current_count; // number of vehicles in the garage in a particular time
    RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus{
    Garage** garages; //list of garages in campus
    int total_garages; //number of garages in the campus
    RegisteredVehicle** registered_vehicles; //list of registered vehicles
    int total_registered_vehicles; //total number of registered vehicles
} Campus;


Garage* createGarage(const char* name, int capacity){
    // Allocate memory for the garage array
    Garage *garage = malloc(sizeof(struct Garage));

    // Allocate memory for the garage name.
    garage->garage_name = malloc(sizeof(char) * (strlen(name) + 1)); 
    strcpy(garage->garage_name, name); // Copy given garage name into array

    garage->total_capacity = capacity;
    garage->current_count = 0;

    // Allocate memory for the parked_vehicles array
    garage->parked_vehicles = malloc(sizeof(RegisteredVehicle*) * capacity);

    return garage;
}

RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license) {
    for (int i = 0; i < campus->total_registered_vehicles; i++)
    {
        if (strcmp((campus->registered_vehicles[i]->license_plate), license) == 0)
        {
            printf("%s\n",campus->registered_vehicles[i]->owner_name);
            return campus->registered_vehicles[i];
        } 
    }
    return NULL;
}

RegisteredVehicle* createRegisteredVehicle (const char* license, const char* owner) {
    // Allocate memory for the registered vehicle array
    RegisteredVehicle *vehicle = malloc(sizeof(RegisteredVehicle));
    // Allocate memory for the vehicle's license plate and owner name
    vehicle->license_plate = malloc(sizeof(char) * (strlen(license) + 1));
    strcpy(vehicle->license_plate, license); // Copy given license into array 

    vehicle->owner_name = malloc(sizeof(char) * (strlen(owner) + 1));
    strcpy(vehicle->owner_name, owner); // Copy given name into array 
    return vehicle;
}

void registerVehicle(Campus* campus, const char* license, const char* owner) {
    // Reallocate memory to fit the new vehicle into the campus' registered vehicles array 

    campus->registered_vehicles = realloc(campus->registered_vehicles, (sizeof(RegisteredVehicle*) * (campus->total_registered_vehicles+ 1)));
    RegisteredVehicle* new_vehicle = createRegisteredVehicle(license,owner);
    campus->registered_vehicles[campus->total_registered_vehicles] = new_vehicle;
    campus->total_registered_vehicles++;
    printf("REGISTERED\n");      
}

void parkVehicle(Garage* garage, RegisteredVehicle* vehicle) {
    if (garage->current_count == garage->total_capacity)
    {
        printf("FULL\n");
    } else {

        // ParkedVehicles[last index] == vehicle given
        garage->parked_vehicles[garage->current_count] = vehicle;
        garage->current_count++;
        printf("PARKED\n");
    }
}

//This one doesn't prints REMOVE FROM GARAGE n
int removeVehicleFromGarage2 (Garage* garage, const char* license) {
    // Iterate through the number of cars parked in the garage
    for (int i = 0; i < garage->current_count; i++)
    {
        // Check if the license [i] is the same as the given one
        if (strcmp(garage->parked_vehicles[i]->license_plate, license) == 0)
        {
            // Shift the vehicles to fill the gap
            for (int j = i; j < garage->current_count - 1; j++) {
                garage->parked_vehicles[j] = garage->parked_vehicles[j + 1];
            }
            garage->parked_vehicles[garage->current_count - 1] = NULL;
            garage->current_count--;
            return 1;
        } else{ 
        printf("NOT FOUND IN CAMPUS\n");
        }
    }
    return 0;
}

int removeVehicleFromGarage(Garage* garage, const char* license) {
    // Iterate through the number of cars parked in the garage
    for (int i = 0; i < garage->current_count; i++)
    {
        // Check if the license [i] is the same as the given one
        if (strcmp(garage->parked_vehicles[i]->license_plate, license) == 0)
        {
            // Shift the vehicles to fill the gap
            for (int j = i; j < garage->current_count - 1; j++) {
                garage->parked_vehicles[j] = garage->parked_vehicles[j + 1];
            }
            garage->parked_vehicles[garage->current_count - 1] = NULL;
            garage->current_count--;
            printf("REMOVED FROM %s\n", garage->garage_name);
            return 1;
        } 
        
    }
    return 0;
}

// returns vehicles PARKED, not registered
int countTotalVehicles(const Campus* campus) {
    int total = 0;
    for (int i = 0; i < campus->total_garages; i++)
    {
        total += campus->garages[i]->current_count;
    }
    return total;
}

int resizeGarage(Garage* garage, int new_capacity) { 
    if (garage->current_count > new_capacity) {
        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;
    } else { 
        // Reallocate space for the vehicle array inside the garage given
        garage->parked_vehicles = realloc(garage->parked_vehicles, sizeof(RegisteredVehicle*) * new_capacity);

        garage->total_capacity = new_capacity;
        printf("SUCCESS\n");
        return 1;
    }
}

int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name) {  
    Garage* target_garage = NULL; 
    Garage* current_garage = NULL; 
    RegisteredVehicle* target_vehicle = NULL;
    int flagVar = 0;

    // Identify target garage and current garage
    for (int i = 0; i < campus->total_garages; i++)
    {
        if (strcmp(campus->garages[i]->garage_name, target_garage_name)==0)
        {
            target_garage = campus->garages[i];
        }
        //target garage is correctly located
        
        
        for (int j = 0; j < campus->garages[i]->current_count; j++)
        {
        
            if (strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, license) == 0)
            {
                current_garage = campus->garages[i];
                target_vehicle = campus->garages[i]->parked_vehicles[j];

                break;
            }   
        }
        
    }
    
    if (target_garage == NULL)
    {
        printf("%s NOT FOUND.\n", target_garage_name);
        return 0;
    }
    if (target_vehicle == NULL) {
        printf("%s NOT IN CAMPUS.\n", license);
        return 0;
    }
    
    if (target_garage->current_count == target_garage->total_capacity)
    {
        printf("%s IS FULL.\n", target_garage_name);
        return 0;
    }

    // Retrieve target vehicle at the garage it is parked at
    
    // for (int i = 0; i < current_garage->current_count; i++)
    // {
    //     if (strcmp(current_garage->parked_vehicles[i]->license_plate, license) == 0)
    //     {
    //         target_vehicle = current_garage->parked_vehicles[i];
    //         flagVar = 1;
    //         break;         
    //     }
    // }


    
    removeVehicleFromGarage2(current_garage, target_vehicle->license_plate);
    parkVehicle(target_garage, target_vehicle);
    printf("RELOCATION SUCCESSFUL.\n");
    return 1;
}

void displayVehiclesByOwner(const Campus* campus, const char* owner_name){ 
    char target_lp[22] = "";

    for (int k = 0; k < campus->total_registered_vehicles; k++)
    {
        if (strcmp(owner_name, campus->registered_vehicles[k]->owner_name)==0)
        {
            int flagVar = 0;
            strcpy(target_lp, campus->registered_vehicles[k]->license_plate);
            for (int i = 0; i < campus->total_garages; i++)
            {
                if (campus->garages[i]->current_count > 0)
                {
                    for (int j = 0; j < campus->garages[i]->current_count; j++)
                    {
                        // NULL BECAUSE TARGET VEHICLE IS NOT PARKED 
                        // printf("%p\n", campus->garages[i]->parked_vehicles[j]);

                        if (campus->garages[i]->parked_vehicles[j] == NULL)
                        {
                            continue;
                        }
                        if (strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, target_lp) == 0){
                            flagVar = 1;
                            printf("%s %s\n", campus->garages[i]->parked_vehicles[j]->license_plate, campus->garages[i]->garage_name);
                            break;
                        } 
                    }
                }
                
            }  
            if (flagVar == 0)
            {
                printf("%s NOT ON CAMPUS\n", target_lp);
            }
        }
    }

    if (target_lp[0] == '\0')
    {
        printf("NO REGISTERED CAR BY THIS OWNER\n");
    }
}

int removeGarage(Campus* campus, const char* garage_name){
    bool garage_found = false;
    for (int i = 0; i < campus->total_garages; i++)
    {
        if (strcmp(campus->garages[i]->garage_name, garage_name)==0)
        {
            free(campus->garages[i]->parked_vehicles);
            free(campus->garages[i]->garage_name);
            free(campus->garages[i]);

            // Shift the garages array to fill the gap
            for (int j = i; j < campus->total_garages - 1; j++) {
                campus->garages[j] = campus->garages[j + 1];
            }

            // Nullify the last garage
            campus->garages[campus->total_garages - 1] = NULL;
            campus->total_garages--;
            
            printf("%s REMOVED\n", garage_name);
            garage_found = true;
            return 1;
        }
    }
    if (!garage_found)
    {
        printf("%s NOT FOUND\n", garage_name);
        return 0;
    }
    return 1;
}

void generateGarageUtilizationReport(const Campus* campus) {
    double min_utilization = 100.0;
    int min_utilization_index = 0;
    for (int i = 0; i < campus->total_garages; i++)
    {
        double utilization = 100.0 * ( (double)campus->garages[i]->current_count / campus->garages[i]->total_capacity);
        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.2f%%\n", 
        campus->garages[i]->garage_name,campus->garages[i]->total_capacity,campus->garages[i]->current_count, utilization);

        if (utilization < min_utilization)
        {
            min_utilization = utilization;
            min_utilization_index = i;
        }
    }
    printf("Least Utilized: %s\n", campus->garages[min_utilization_index]->garage_name);
}

void free_up_memory(Campus* campus) {
    // Free up vehicles array
    for (int i = 0; i < campus->total_registered_vehicles; i++)
    {
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
        free(campus->registered_vehicles[i]);
    }
    // Free up garages array
    for (int i = 0; i < campus->total_garages; i++)
    {
        free(campus->garages[i]->garage_name);
        free(campus->garages[i]->parked_vehicles);
        free(campus->garages[i]);
    }
    free(campus->garages);
    free(campus->registered_vehicles);
}

int main(void) {
    int g, v, c;
    scanf("%d %d %d", &g, &v, &c);
    Campus campus;
    campus.total_garages = g;
    campus.total_registered_vehicles = v;
    // Allocate memory for all garages and vehicles in campus
    campus.garages = malloc(g*sizeof(Garage*));
    campus.registered_vehicles = malloc(v*sizeof(RegisteredVehicle*));

    // Create g garages
    for (int i = 0; i < g; i++)
    {
        char name[22];
        int capacity;
        scanf("%s %d", name, &capacity);
        campus.garages[i] = createGarage(name, capacity);
    }

    // Create v registered vehicles
    for (int i = 0; i < v; i++)
    {
        char license_plate[22];
        char owner_name[22];
        scanf("%s %s", license_plate, owner_name);
        campus.registered_vehicles[i] = createRegisteredVehicle(license_plate, owner_name);
    }

    char input[100]; // Line of input
    char command[22];  
    char arg1[22], arg2[22];
    int arg3;
    // Execute all c commands
    for (int i = 0; i < c+1; i++)
    {
        fgets(input, sizeof(input) + 1, stdin);
        // Identify the command
        sscanf(input, "%s", command);
        if (strcmp(command, "PARK") == 0) {
                                           //  LP    Gn
            sscanf(input, "%s %s %s", command, arg1, arg2); 
            Garage* target_garage = NULL; 
            RegisteredVehicle* target_vehicle = NULL;
            for (int j = 0; j < campus.total_registered_vehicles; j++)
            {
                if (strcmp(campus.registered_vehicles[j]->license_plate,arg1)==0) {
                    target_vehicle = campus.registered_vehicles[j];
                }     
            }
            for (int i = 0; i < campus.total_garages; i++) {
                // Check if target garage is in garages array
                if (strcmp(campus.garages[i]->garage_name,arg2)==0) {
                    target_garage = campus.garages[i];
                }
            }
            parkVehicle(target_garage, target_vehicle);
        }
        if (strcmp(command, "UTILIZATION_REPORT") == 0)
        {
            generateGarageUtilizationReport(&campus);
        }
        if (strcmp(command, "RESIZE")==0)
        {
            //RESIZE GARAGE_C 2
                                            // Gn    Nc
            sscanf(input, "%s %s %d", command, arg1, &arg3);
            for (int i = 0; i < campus.total_garages; i++)
            {
                if (strcmp(campus.garages[i]->garage_name, arg1)==0)
                {
                    resizeGarage(campus.garages[i], arg3);
                    break;
                }
                
            }
        }
        if (strcmp(command, "SEARCH_OWNER")==0) {
                                        //  owner     
            sscanf(input, "%s %s", command, arg1);
            displayVehiclesByOwner(&campus, arg1);
        }
        if (strcmp(command, "RELOCATE")==0) {
                                            // LP    G
            sscanf(input, "%s %s %s", command, arg1, arg2);
            //int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name) {  
            relocateVehicle(&campus, arg1, arg2);
        }
        if (strcmp(command, "COUNT_TOTAL")==0){
            printf("%d\n", countTotalVehicles(&campus));
        }
        if (strcmp(command, "REGISTER_VEHICLE")==0){ 
                                             // LP   Owner
            sscanf(input, "%s %s %s", command, arg1, arg2);
            registerVehicle(&campus, arg1, arg2);
        }
        if (strcmp(command, "REMOVE_VEHICLE_GARAGE")==0){ 
                                         // Lp
            sscanf(input, "%s %s", command, arg1);
            int found = 0;
            for (int i = 0; i < campus.total_garages; i++) {
                if (removeVehicleFromGarage(campus.garages[i], arg1)) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                printf("NOT FOUND IN CAMPUS\n");
            }
            
        }
        if (strcmp(command, "REMOVE_GARAGE")==0){ 
                                         // G
            sscanf(input, "%s %s", command, arg1);
            removeGarage(&campus, arg1);        
        }
    }
    free_up_memory(&campus);
    return 0;
}