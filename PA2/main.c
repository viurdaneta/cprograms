/* COP 3502C Assignment 1
This program is written by:  Victor Urdaneta */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Student node
typedef struct Student
{
  int sequenceNumber;
  struct Student* next;
} Student;

//defining the structure for the queue
typedef struct Queue
{
    Student* front;
    Student* back; 
    int nodeCount;
    int k;
    int th;
    int garageNumber;    

} Queue;

// dynamically allocate a
// Student structure and returns a Student node 
Student* createStudent(int sequence) {
    Student* st = malloc(sizeof(Student));
    if (!st) {
        return NULL; // Memory allocation failed
    }
	st->sequenceNumber = sequence;
	st->next = NULL;
	return st;
}

void init(Queue* q, int garageNumber, int numStudents, int k, int th) {
	
	//malloc for the array based on initial max size defined above
	//initialize the other queue properties apprpriately
    q->front = NULL;
    q->back = NULL;
    q->garageNumber = garageNumber;   
    q->nodeCount = numStudents;
    q->k = k;
    q->th = th;
}

int empty(struct Queue* qPtr) {
   if (qPtr->front == NULL && qPtr->back == NULL)
   {
        return 1;
   } 
   else {
        return 0;
   }

   
}

void display(Queue *q) {
    if (empty(q)) {
        printf("Queue is empty\n");
        return;
    }

    Student* current = q->front;
    // Traverse the circular linked list
    printf("%d ", q->garageNumber);
    do
    {
        printf("%d ", current->sequenceNumber);
        current = current->next;
    } while (current != q->front);
    
    printf("\n");
}

//takes the reference of a queue and insert the item in the queue
int enQueue(Queue *q, int seq_number)
{
    Student* newStudent = createStudent(seq_number);
    if (empty(q))
    {
        // Circular queue
        q->front = newStudent;
        q->back = newStudent;
        newStudent->next = newStudent;
    }
    else {
        // Add new student to the back of the queue
        newStudent->next = q->front;
        q->back->next = newStudent;
        q->back = newStudent;
    }
    return 1;
} /*End of insert()*/

//returning the front item if not empty and increase front
int deQueue(Queue *q)
{
    Student* removedStudent = q->front;
    if (q->front == q->back) // Empty queue
    {
        printf("Queue is empty\n");
        return 0;
    } 
    else 
    {
        q->back->next = q->front->next;
        q->front = q->front->next;
    }

    q->nodeCount--;
    removedStudent->next = NULL; // Detach from the queue
    return removedStudent->sequenceNumber;

} /*End of delete() */


void createReverseCircle(Queue* q) {
    for (int i = q->nodeCount; i > 0; i--) 
    {
        enQueue(q, i); // Add students in reverse order
    }
    // Make queue circular
    if (q->back != NULL)
    {
        q->back->next = q->front;
    }
}


void rearrangeCircle(Queue* q) {
    if (empty(q)) {
        // If the queue is empty or has only one element, no need to reverse
        return;
    }
    Student* prev = NULL;
    Student* current = q->front;
    Student* next = NULL;
    Student* start = q->front;
    do {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;     
    } while (current != start);
    // Front and back pointers
    q->back = q->front;
    q->front = prev;

    // Circular link
    q->back->next = q->front;
    display(q);
}

void phase1elimination(Queue *q) {
    printf("\nGroup for Garage# %d\n", q->garageNumber);
    Student* current = q->front;
    Student* prev = q->back;
    while (q->th < q->nodeCount)
    {
        // Skip k-1 students
        for (int i = 1; i < q->k; i++) {
            prev = current;
            current = current->next;
        }

        printf("Student# %d eliminated\n", current->sequenceNumber);
         // Adjust front and back if necessary
        if (current == q->front) {
            q->front = current->next;
        }
        if (current == q->back) {
            q->back = prev;
        }

        // Remove the current student
        prev->next = current->next;
        free(current);
        current = prev->next;
        q->nodeCount--;
    }
    // Re-establish circular link
    q->back->next = q->front;
}

void phase2elimination(Queue* queues, int numQueues) {
    while (1) {
        Student* maxStudent = NULL;
        Queue* maxQueue = NULL;

        // Find the student with the highest sequence number of the front students of all non-empty queues
        for (int i = 0; i < numQueues; i++) {
            Queue* q = &queues[i];
            if (q->front != NULL) { // Non-empty queue
                Student* frontStudent = q->front;
                if (maxStudent == NULL || 
                   (frontStudent->sequenceNumber > maxStudent->sequenceNumber) ||
                   (frontStudent->sequenceNumber == maxStudent->sequenceNumber && q->garageNumber < maxQueue->garageNumber)) {
                    maxStudent = frontStudent;
                    maxQueue = q;
                }
            }
        }
        // Check if only one student remains
        int remainingStudents = 0;
        for (int i = 0; i < numQueues; i++) {
            if (queues[i].nodeCount > 0) {
                remainingStudents += queues[i].nodeCount;
            }
        }
        if (remainingStudents == 1) {
            break;
        }

        // Eliminate the student with the highest sequence number
        if (maxQueue != NULL) {
            printf("Eliminated student %d from group for garage %d\n", maxStudent->sequenceNumber, maxQueue->garageNumber);
            // Remove the student from the maxQueue
            if (maxQueue->front == maxQueue->back) { // Only one student left in this queue
                maxQueue->front = NULL;
                maxQueue->back = NULL;
            } else {
                maxQueue->front = maxQueue->front->next;
                maxQueue->back->next = maxQueue->front; 
            }
            free(maxStudent);
            maxQueue->nodeCount--;
        }
    }

    for (int i = 0; i < numQueues; i++) {
        if (queues[i].nodeCount == 1) {
            printf("\nStudent %d from the group for garage %d is the winner!", queues[i].front->sequenceNumber, queues[i].garageNumber);
            break;
        }
    }
}

void sortQueuesByGarageNumber(Queue* queues, int G) {
    for (int i = 0; i < G - 1; i++) {
        for (int j = 0; j < G - i - 1; j++) {
            if (queues[j].garageNumber > queues[j + 1].garageNumber) {
                // Swap queues[j] and queues[j + 1]
                Queue temp = queues[j];
                queues[j] = queues[j + 1];
                queues[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    int G;

    // Read the number of garages
    scanf("%d", &G);

    Queue queues[G];

    // Read input and initialize queues
    for (int i = 0; i < G; i++) {
        int garageNumber, numStudents, k, th;
        scanf("%d %d %d %d", &garageNumber, &numStudents, &k, &th);
        // Initialize the queue

        init(&queues[i], garageNumber, numStudents, k, th);

        // Create a circular linked list in reverse order
        createReverseCircle(&queues[i]);
    }
    sortQueuesByGarageNumber(queues, G);
    // Display the initial status of non-empty queues
    printf("Initial status of nonempty queues\n");
    for (int i = 0; i < G; i++) {
        display(&queues[i]);
    }

    // Rearrange the students to ascending order in each queue
    printf("\nAfter ordering status of nonempty queues\n");
    for (int i = 0; i < G; i++) {
        rearrangeCircle(&queues[i]);
    }

    // Perform Phase 1 elimination for each garage
    printf("\nPhase1 elimination\n");
    for (int i = 0; i < G; i++) {
        phase1elimination(&queues[i]);
    }

    // Perform Phase 2 elimination
    printf("\nPhase2 elimination\n");
    phase2elimination(queues, G);

    return 0;
}